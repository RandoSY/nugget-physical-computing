#!/usr/bin/env python3
"""Host C syntax-only check, NOT an MSP430 executable build.
Run: python3 tools/check_sources.py ; requires local host gcc.
"""
from pathlib import Path
import subprocess, sys
root = Path(__file__).resolve().parents[1]
header = root / 'tools' / 'mock' / 'msp430.h'
files = sorted((root / 'lessons').glob('*/main.c')) + sorted((root / 'lessons' / '11-tiny-multitasker').glob('0*/main.c')) + sorted((root / 'appendices').glob('*/main.c')) + sorted((root / 'evolution').glob('*/[0-9]*/main.c'))
failed=[]
for path in files:
    cmd=['gcc','-std=c99','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-fsyntax-only','-I',str(header.parent),str(path)]
    p=subprocess.run(cmd,text=True,capture_output=True)
    print(('PASS' if p.returncode==0 else 'FAIL'),path.relative_to(root))
    if p.returncode: failed.append((path,p.stderr))
for path,error in failed: print(path,'\n',error,file=sys.stderr)
print(f'Host syntax checks: {len(files)-len(failed)}/{len(files)} passed; target toolchain and electronics NOT checked.')
sys.exit(1 if failed else 0)
