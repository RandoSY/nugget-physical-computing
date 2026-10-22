# Integrated Curriculum v1.0 (2026-10-22)

## Restored from original repository

30 unmodified MSP430 `.asm` files from ADolbyB/msp430g2553-labs, all matching original Git blobs. The original assembly files are committed to the GitHub tree under `historical/original-assembly/Assembly-Lang/`.

## Newly created integration material

- Sixteen-week course map, JSON index and 16 detailed week plans.
- Instructor guide PDF, course strategy, architecture comparison, assessment and instrument capstone.
- Bridge 01 LED C / MSP430 assembly, bridge 02 bit-counting C / assembly. These are **new code**, not modifications to original sources.
- ASM recovery manifest and provenance details.

## Unchanged source libraries

The 11 C lessons, four sensor appendices, Edu430 FwLib v0.2, the VL6180X v0.3 reference, and previously published teacher PDFs are retained unchanged.

## Verified

- 30/30 original ASM Git blob SHA matches.
- 21/21 core C host syntax checks.
- 14/14 Edu430 host checks.
- 2/2 bridge C host syntax checks.
- Bit-count reference model passes representative and randomized integer cases.
- Integrated instructor PDF renders and is 15 pages.

## Not verified

- Assembly compiling on genuine TI MSP430 assembler / CCS 21; physical LED and sensor qualification.
- PDP-11 assembly execution; comparison is conceptual based on primary documentation.

## Best self-contained download

The **GitHub main branch archive** contains all 30 original `.asm` files and all course content:
https://github.com/RandoSY/nugget-physical-computing/archive/refs/heads/main.zip

The separately downloadable *local edition* ZIP omits the upstream ASM bytes and contains an exact source manifest plus instructions for obtaining the complete GitHub tree.
