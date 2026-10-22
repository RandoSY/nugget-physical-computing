# PDP-11 and MSP430 — Architecture Comparison

The PDP-11 is a **comparative model**, not a claim that MSP430 assembly runs on DEC hardware or that the instruction sets are identical. Use register traces and machine state to distinguish superficial mnemonic similarities from actual ISA semantics.

## Common conceptual foundation

Both are byte/word-oriented **16-bit architectures** with general registers, a stack, condition codes, source/destination instructions, memory-mapped I/O and several addressing modes. Both can express `MOV source,destination`, `ADD`, `BIC`, `BIS` and branches naturally. Instruction names alone do not imply identical encodings, cycle timing or status-flag rules.

| Property | MSP430G2553 | Classic PDP-11 |
|---|---|---|
| CPU programmer-visible register set | 16 × 16-bit registers | 8 × 16-bit architectural registers |
| PC | R0 | R7 |
| SP | R1 | R6 |
| Special registers | R2 status/constant generator; R3 constant generator | R0–R5 commonly general purpose, R6 SP, R7 PC; separate PSW |
| General registers most often used | R4–R15 | R0–R5 |
| Arithmetic and data widths | 8- and 16-bit `.B` / `.W` | 8- and 16-bit `B` suffix / default word |
| Typical operand order | `MOV.W #3,R5` | `MOV #3,R5` |
| Immediate | `#value` | `#value` |
| Register indirect | `@R5` | `(R5)` |
| Post-increment | `@R5+` | `(R5)+` |
| Indexed | `2(R5)` | `2(R5)` |
| Memory-mapped I/O | Yes | Yes (UNIBUS/Q-bus memory and device address space, model-dependent) |
| User-visible condition codes | N,Z,C,V in status register | N,Z,V,C in PSW |
| Assembly numbers | TI syntax allows decimal, hexadecimal `0x` or `...h` | MACRO-11 conventional default is octal; explicit radix required for decimal/hex |
| Instruction equivalence | Not binary compatible | Not binary compatible |

## Safe direct comparison: register-only arithmetic

MSP430 TI assembler:

```asm
    mov.w   #3,R5
    add.w   #2,R5
    bic.w   #1,R5
```

Illustrative PDP-11 MACRO-11 source (these small constants have the same values in octal and decimal):

```asm
    MOV     #3,R5
    ADD     #2,R5
    BIC     #1,R5
```

Both leave R5 equal to 4. Instruction encoding, cycles and flags must be checked in the appropriate machine manual, not assumed. Do not use `#10` as a comparison without clarifying PDP-11's octal default.

## Flag rules: essential subtlety

An MSP430 `CMP src,dst` follows `dst-src` for flags, but some example comments in old assembler tutorials reverse it. For PDP-11, `CMP src,dst` is also evaluated as `src-dst` for condition codes. **Do not translate branch conditions mechanically**; test with positive, negative and equal operands. Instructors should demonstrate this difference explicitly using small values and the IDE registers window.

## Addressing-mode laboratory

Use the existing [`SimpleVariables.asm`](../../historical/original-assembly/Assembly-Lang/SimpleVariables.asm) first, then `Array-Functions/FindMinArrayIndex.asm`. Put R5 at a safe RAM address in a debugger exercise, inspect `@R5+` and `2(R5)`, and compare with PDP-11 `(R5)+` and `2(R5)`. Explicitly describe what gets advanced and by how many bytes.

## Stack laboratory

The MSP430's R1 SP is not PDP-11 R6 SP. On the MSP430 `PUSH.W` decrements SP and stores a word; `CALL` and `RET` change it. Similar *concept*, different instruction form and calling convention. The historical `SampleInstructionSet.asm` contains comments that are not a substitute for debugger observation.

## Recommended primary references

- Texas Instruments, *MSP430x2xx Family User's Guide*, SLAU144 (CPU and instruction-set sections): https://www.ti.com/lit/ug/slau144/slau144.pdf
- Digital Equipment Corporation, *PDP-11/40 Processor Handbook*: https://pdos.lcs.mit.edu/6.828/2003/readings/pdp11-40.pdf
- Digital Equipment Corporation, *PDP-11 Architecture Handbook (1983)*: https://bitsavers.computerhistory.org/pdf/dec/pdp11/handbooks/EB-23657-18_PDP-11_Architecture_Handbook_1983.pdf

## Historical correction principle

Do not edit the original assembly archive. Put all corrections in modern bridge labs or instructor notes. For example, the old `CountBinary-1s.asm` describes a rightward rotation while the listing uses `RLA.W`, a **leftward arithmetic shift**. That is a teachable documentation/algorithm discrepancy. Our bridge lab provides a tested logical-right-shift algorithm without replacing the source.
