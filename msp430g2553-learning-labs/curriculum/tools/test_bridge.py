#!/usr/bin/env python3
"""Reference model; does not build or simulate the TI assembler."""

def ones_by_shift(v):
    assert 0 <= v <= 0xFFFF
    count=0
    for _ in range(16):
        carry=v & 1
        v=(v>>1)&0x7FFF
        count+=carry
    return count

for v in [0,1,0x8000,0xEDCB,0xFFFF,0x5555,0xAAAA]+list(range(0,4096,17)):
    assert ones_by_shift(v)==v.bit_count(),hex(v)
print('PASS: all reference values, including sign-bit and zero cases')
