#!/usr/bin/env python3
"""Verifies host C syntax of ALL fwlib examples; no MSP430 compiler claimed.
Examples are compiled in isolation because each defines its own main().
"""
from pathlib import Path
import subprocess,sys,tempfile
base=Path(__file__).resolve().parents[1]
mock=base/'tests'/'mock'; includes=base/'include'
examples=sorted((base/'examples').glob('*/main.c'))
fail=[]
for f in examples:
    cmd=['gcc','-std=c99','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas', '-I',str(mock),'-I',str(includes),'-fsyntax-only',str(f)]
    r=subprocess.run(cmd,capture_output=True,text=True)
    print(('PASS' if r.returncode==0 else 'FAIL'),f.parent.name)
    if r.returncode:fail.append((str(f),r.stderr))
with tempfile.TemporaryDirectory() as temp:
    for f in sorted((base/'tests').glob('test_*.c')):
        exe=Path(temp)/f.stem
        cmd=['gcc','-std=c99','-Wall','-Wextra','-Werror','-I',str(mock),'-I',str(includes),str(f),'-o',str(exe)]
        r=subprocess.run(cmd,capture_output=True,text=True)
        if r.returncode==0:r=subprocess.run([str(exe)],capture_output=True,text=True)
        print(('PASS' if r.returncode==0 else 'FAIL'),f.name)
        if r.returncode:fail.append((str(f),r.stderr))
for f,e in fail:print(f,e,file=sys.stderr)
print(f'{len(examples)+len(list((base/"tests").glob("test_*.c")))-len(fail)} tests passed. Host mock does not prove TI CCS builds or electrical function.')
raise SystemExit(bool(fail))
