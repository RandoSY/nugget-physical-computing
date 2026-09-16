#!/usr/bin/env python3
"""UNO Laboratory Workstation local bridge.

Binds to loopback only and exposes a deliberately small API used by the browser IDE:
  GET  /api/health
  GET  /api/boards
  POST /api/compile
  POST /api/upload
  GET  /             serves the workstation HTML next to this script

No shell is used when invoking Arduino CLI. Sketches compile in temporary directories.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
import webbrowser
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any

HOST = "127.0.0.1"
DEFAULT_PORT = 7123
MAX_SOURCE_BYTES = 1_000_000
MAX_BODY_BYTES = 1_200_000
FQBN_RE = re.compile(r"^[A-Za-z0-9_.-]+:[A-Za-z0-9_.-]+:[A-Za-z0-9_.=-]+(?::[A-Za-z0-9_.=,-]+)?$")
PORT_RE = re.compile(r"^(?:COM\d+|/dev/[A-Za-z0-9._/+:-]+)$", re.I)
SAFE_NAME_RE = re.compile(r"[^A-Za-z0-9_-]+")


def find_cli(explicit: str | None = None) -> Path | None:
    candidates: list[Path] = []
    if explicit:
        candidates.append(Path(explicit).expanduser())
    env_cli = os.environ.get("ARDUINO_CLI")
    if env_cli:
        candidates.append(Path(env_cli).expanduser())
    which = shutil.which("arduino-cli") or shutil.which("arduino-cli.exe")
    if which:
        candidates.append(Path(which))

    if os.name == "nt":
        roots = [
            os.environ.get("LOCALAPPDATA"),
            os.environ.get("PROGRAMFILES"),
            os.environ.get("PROGRAMFILES(X86)"),
        ]
        rels = [
            Path("Programs/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe"),
            Path("Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe"),
        ]
        for root in roots:
            if root:
                for rel in rels:
                    candidates.append(Path(root) / rel)

    for p in candidates:
        try:
            if p.is_file():
                return p.resolve()
        except OSError:
            pass
    return None


def run_cmd(args: list[str], timeout: int = 90) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        args,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=timeout,
        shell=False,
        creationflags=(subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0),
    )


def cli_version(cli: Path) -> str:
    try:
        r = run_cmd([str(cli), "version"], timeout=10)
        line = (r.stdout or r.stderr).strip().splitlines()
        return line[0] if line else "unknown"
    except Exception as e:
        return f"unavailable: {e}"


def normalize_board_list(raw: Any) -> list[dict[str, Any]]:
    if isinstance(raw, dict):
        items = raw.get("detected_ports") or raw.get("ports") or []
    elif isinstance(raw, list):
        items = raw
    else:
        items = []

    out: list[dict[str, Any]] = []
    for item in items:
        if not isinstance(item, dict):
            continue
        port_obj = item.get("port") if isinstance(item.get("port"), dict) else item
        address = port_obj.get("address") or item.get("address")
        if not address:
            continue
        boards = item.get("matching_boards") or item.get("boards") or []
        board_names: list[str] = []
        fqbns: list[str] = []
        for b in boards if isinstance(boards, list) else []:
            if isinstance(b, dict):
                if b.get("name"):
                    board_names.append(str(b["name"]))
                if b.get("fqbn"):
                    fqbns.append(str(b["fqbn"]))
        out.append({
            "address": str(address),
            "label": str(port_obj.get("label") or address),
            "protocol": str(port_obj.get("protocol") or item.get("protocol") or ""),
            "boards": board_names,
            "fqbns": fqbns,
        })
    return out


def sketch_basename(file_name: str) -> str:
    stem = Path(file_name or "Sketch.ino").stem
    stem = SAFE_NAME_RE.sub("_", stem).strip("_")[:48]
    if not stem or not stem[0].isalpha():
        stem = "Sketch_" + stem
    return stem or "Sketch"


def validate_payload(data: dict[str, Any]) -> tuple[str, str, str, str]:
    source = data.get("source")
    fqbn = data.get("fqbn") or "arduino:avr:uno"
    file_name = data.get("fileName") or "Sketch.ino"
    port = data.get("port") or ""
    if not isinstance(source, str):
        raise ValueError("source must be text")
    if len(source.encode("utf-8")) > MAX_SOURCE_BYTES:
        raise ValueError("source is too large")
    if not isinstance(fqbn, str) or not FQBN_RE.match(fqbn):
        raise ValueError("invalid FQBN")
    if port and (not isinstance(port, str) or not PORT_RE.match(port)):
        raise ValueError("invalid upload port")
    return source, fqbn, str(file_name), str(port)


def compile_sketch(cli: Path, source: str, fqbn: str, file_name: str) -> dict[str, Any]:
    name = sketch_basename(file_name)
    with tempfile.TemporaryDirectory(prefix="uno-lab-") as td:
        root = Path(td)
        sketch_dir = root / name
        output_dir = root / "build"
        sketch_dir.mkdir()
        output_dir.mkdir()
        sketch_file = sketch_dir / f"{name}.ino"
        sketch_file.write_text(source, encoding="utf-8", newline="\n")

        cmd = [str(cli), "compile", "--fqbn", fqbn, "--output-dir", str(output_dir), str(sketch_dir)]
        started = time.perf_counter()
        r = run_cmd(cmd, timeout=120)
        elapsed = time.perf_counter() - started
        output = "\n".join(x for x in [r.stdout.strip(), r.stderr.strip()] if x).strip()
        if r.returncode != 0:
            return {"ok": False, "returncode": r.returncode, "output": output, "elapsed": elapsed, "command": cmd[1:]}

        hex_files = [p for p in output_dir.glob("*.hex") if "with_bootloader" not in p.name.lower()]
        if not hex_files:
            hex_files = list(output_dir.glob("*.hex"))
        if not hex_files:
            return {"ok": False, "returncode": 0, "output": output + "\nCompile succeeded but no Intel HEX file was found.", "elapsed": elapsed, "command": cmd[1:]}
        hex_path = sorted(hex_files, key=lambda p: ("with_bootloader" in p.name.lower(), len(p.name)))[0]
        hex_text = hex_path.read_text(encoding="ascii", errors="strict")
        return {
            "ok": True,
            "returncode": 0,
            "output": output,
            "elapsed": elapsed,
            "command": cmd[1:],
            "hex": hex_text,
            "hexFile": hex_path.name,
            "hexBytes": len(hex_text.encode("ascii")),
        }


def compile_and_upload(cli: Path, source: str, fqbn: str, file_name: str, port: str) -> dict[str, Any]:
    if not port:
        raise ValueError("select an UNO serial port before upload")
    name = sketch_basename(file_name)
    with tempfile.TemporaryDirectory(prefix="uno-lab-") as td:
        root = Path(td)
        sketch_dir = root / name
        output_dir = root / "build"
        sketch_dir.mkdir()
        output_dir.mkdir()
        (sketch_dir / f"{name}.ino").write_text(source, encoding="utf-8", newline="\n")

        compile_cmd = [str(cli), "compile", "--fqbn", fqbn, "--output-dir", str(output_dir), str(sketch_dir)]
        started = time.perf_counter()
        rc = run_cmd(compile_cmd, timeout=120)
        compile_out = "\n".join(x for x in [rc.stdout.strip(), rc.stderr.strip()] if x).strip()
        if rc.returncode != 0:
            return {"ok": False, "stage": "compile", "returncode": rc.returncode, "output": compile_out, "elapsed": time.perf_counter()-started, "command": compile_cmd[1:]}

        upload_cmd = [str(cli), "upload", "--fqbn", fqbn, "--port", port, "--input-dir", str(output_dir), "--verify", str(sketch_dir)]
        ru = run_cmd(upload_cmd, timeout=120)
        upload_out = "\n".join(x for x in [ru.stdout.strip(), ru.stderr.strip()] if x).strip()
        joined = (compile_out + ("\n" if compile_out and upload_out else "") + upload_out).strip()
        return {
            "ok": ru.returncode == 0,
            "stage": "upload" if ru.returncode else "complete",
            "returncode": ru.returncode,
            "output": joined,
            "elapsed": time.perf_counter()-started,
            "command": upload_cmd[1:],
            "port": port,
        }


class BridgeState:
    def __init__(self, cli: Path | None, ui_path: Path):
        self.cli = cli
        self.ui_path = ui_path
        self.version = "0.4.0"


class Handler(BaseHTTPRequestHandler):
    server_version = "UnoLabBridge/0.4"

    @property
    def state(self) -> BridgeState:
        return self.server.state  # type: ignore[attr-defined]

    def log_message(self, fmt: str, *args: Any) -> None:
        sys.stdout.write("[%s] %s\n" % (self.log_date_time_string(), fmt % args))

    def _cors(self) -> None:
        origin = self.headers.get("Origin")
        if origin in (None, "null") or origin.startswith("http://127.0.0.1") or origin.startswith("http://localhost"):
            self.send_header("Access-Control-Allow-Origin", origin or "*")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")

    def _json(self, code: int, payload: dict[str, Any]) -> None:
        body = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(code)
        self._cors()
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _read_json(self) -> dict[str, Any]:
        try:
            n = int(self.headers.get("Content-Length", "0"))
        except ValueError:
            raise ValueError("invalid content length")
        if n <= 0 or n > MAX_BODY_BYTES:
            raise ValueError("request body size is invalid")
        raw = self.rfile.read(n)
        try:
            obj = json.loads(raw.decode("utf-8"))
        except Exception as e:
            raise ValueError(f"invalid JSON: {e}")
        if not isinstance(obj, dict):
            raise ValueError("JSON body must be an object")
        return obj

    def do_OPTIONS(self) -> None:
        self.send_response(HTTPStatus.NO_CONTENT)
        self._cors()
        self.end_headers()

    def do_GET(self) -> None:
        if self.path in ("/", "/index.html"):
            try:
                body = self.state.ui_path.read_bytes()
            except OSError as e:
                self._json(500, {"ok": False, "error": f"UI file unavailable: {e}"})
                return
            self.send_response(200)
            self._cors()
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return

        if self.path == "/api/health":
            cli = self.state.cli
            self._json(200, {
                "ok": cli is not None,
                "version": self.state.version,
                "cliFound": cli is not None,
                "cliPath": str(cli) if cli else "",
                "cliVersion": cli_version(cli) if cli else "Arduino CLI not found",
                "host": HOST,
            })
            return

        if self.path == "/api/boards":
            cli = self.state.cli
            if not cli:
                self._json(503, {"ok": False, "error": "Arduino CLI not found. Put arduino-cli on PATH, set ARDUINO_CLI, or start bridge with --cli PATH."})
                return
            try:
                r = run_cmd([str(cli), "board", "list", "--json"], timeout=15)
                if r.returncode != 0:
                    self._json(500, {"ok": False, "error": (r.stderr or r.stdout).strip() or "board list failed"})
                    return
                raw = json.loads(r.stdout or "[]")
                self._json(200, {"ok": True, "ports": normalize_board_list(raw)})
            except Exception as e:
                self._json(500, {"ok": False, "error": str(e)})
            return

        self._json(404, {"ok": False, "error": "not found"})

    def do_POST(self) -> None:
        if self.path not in ("/api/compile", "/api/upload"):
            self._json(404, {"ok": False, "error": "not found"})
            return
        cli = self.state.cli
        if not cli:
            self._json(503, {"ok": False, "error": "Arduino CLI not found. Put arduino-cli on PATH, set ARDUINO_CLI, or start bridge with --cli PATH."})
            return
        try:
            data = self._read_json()
            source, fqbn, file_name, port = validate_payload(data)
            if self.path == "/api/compile":
                result = compile_sketch(cli, source, fqbn, file_name)
            else:
                result = compile_and_upload(cli, source, fqbn, file_name, port)
            status = 200 if result.get("ok") else 400
            self._json(status, result)
        except subprocess.TimeoutExpired:
            self._json(504, {"ok": False, "error": "Arduino toolchain timed out"})
        except ValueError as e:
            self._json(400, {"ok": False, "error": str(e)})
        except Exception as e:
            self._json(500, {"ok": False, "error": f"bridge error: {e}"})


def main() -> int:
    ap = argparse.ArgumentParser(description="UNO Laboratory Workstation Arduino CLI bridge")
    ap.add_argument("--port", type=int, default=DEFAULT_PORT)
    ap.add_argument("--cli", help="Path to arduino-cli executable")
    ap.add_argument("--ui", help="Path to workstation HTML")
    ap.add_argument("--open", action="store_true", help="Open workstation in default browser")
    ns = ap.parse_args()
    if not (1024 <= ns.port <= 65535):
        ap.error("--port must be between 1024 and 65535")

    here = Path(__file__).resolve().parent
    ui = Path(ns.ui).expanduser().resolve() if ns.ui else here / "Embedded_Laboratory_Workstation_v0_4.html"
    cli = find_cli(ns.cli)
    state = BridgeState(cli, ui)

    httpd = ThreadingHTTPServer((HOST, ns.port), Handler)
    httpd.state = state  # type: ignore[attr-defined]
    url = f"http://{HOST}:{ns.port}/"
    print("UNO Laboratory Workstation bridge v0.4")
    print(f"UI:  {ui}")
    print(f"URL: {url}")
    print(f"CLI: {cli if cli else 'NOT FOUND'}")
    if cli:
        print(f"     {cli_version(cli)}")
    else:
        print("Set ARDUINO_CLI or use --cli PATH if Arduino CLI is installed elsewhere.")
    print("Loopback only; press Ctrl+C to stop.")

    if ns.open:
        threading.Timer(0.5, lambda: webbrowser.open(url)).start()
    try:
        httpd.serve_forever(poll_interval=0.25)
    except KeyboardInterrupt:
        print("\nStopping bridge.")
    finally:
        httpd.server_close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
