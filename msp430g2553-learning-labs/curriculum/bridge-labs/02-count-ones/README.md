# Bridge 02 — Counting ones across C, assembly and PDP-11

**Problem:** Count the number of set bits in an unsigned 16-bit value. Use 0xEDCB as a worked input, plus 0x0000, 0xFFFF and 0x8000.

The original source at `historical/original-assembly/Assembly-Lang/Count-Binary-1s/CountBinary-1s.asm` describes a rightward rotation but uses `RLA.W` (left arithmetic shift). This bridge program deliberately uses **CLC before each RRC.W**, producing a logical-right shift. The original remains untouched.

`main.c` and `main.asm` are **alternative entry points**, not files to compile into the same target.

## Activity

1. Predict popcount(0xEDCB) in binary on paper. Count set positions.
2. Single-step the shift, copy the low-order bit into the carry flag and increment the count on carry.
3. Compare C's `x >>= 1` with the explicit flag-based `CLRC; RRC.W; JNC` sequence.
4. Explain how the PDP-11 `ASR` and `ROR` differ for sign/carry propagation; do not use a sign-extending arithmetic shift for an unsigned count without accounting for the inserted sign bit.
5. Explain why testing both `0x8000` and `0x0000` catches subtle errors.

**C and MSP430 assembly verification** must be completed in Code Composer Studio. An independent host-side Python reference test is in `curriculum/tools/test_bridge.py`.
