#!/usr/bin/env python3
"""Host-only compile checks, independent of TI's genuine compiler.

Exit success means syntax + state-machine tests, NOT working electrical I2C.
"""
from pathlib import Path
import subprocess, os
ROOT=Path(__file__).resolve().parent.parent
cc=os.environ.get('CC','gcc')
def cmd(args):
 print('+',' '.join(map(str,args)))
 subprocess.run(args,check=True)
mock=ROOT/'tests/mock'
include=ROOT/'include'
example=ROOT/'examples/09-vl6180x-hardware-i2c'
for x in (example/'main.c',):
 cmd([cc,'-std=c11','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-fsyntax-only',f'-I{mock}',f'-I{include}',str(x)])
for src in ('test_vl6180x.c','test_hw_i2c_guard.c'):
 exe=ROOT/'tests'/src.replace('.c','.bin')
 cmd([cc,'-std=c11','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',f'-I{mock}',f'-I{include}',str(ROOT/'tests'/src),'-o',str(exe)])
 try:cmd([str(exe)])
 finally:exe.unlink(missing_ok=True)
print('PASS: host syntax + independent device behavior checks')
