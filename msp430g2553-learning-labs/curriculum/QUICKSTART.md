# Quick start — choose a route, but not a separate curriculum

**New learners:** Start with `lessons/01-blink`, a simple CCS target project for MSP430G2553, then follow weeks 1–3. Use the comments to connect macro-to-register-to-pin. The program does not require the advanced library.

**C programmers:** Start with `curriculum/bridge-labs/01-bits-and-registers`, then weeks 4–9 to understand flags, shifts, subroutines and memory. Build C and assembly as separate targets.

**Firmware/physical-science instructors:** Start weeks 10–16, integrating sensor calibration, I2C with VL6180X, serial logs, scheduler timing and optional EnergyTrace. See `curriculum/docs/CAPSTONE.md`.

**PDP-11 enthusiasts:** Start with `curriculum/docs/PDP11_ARCHITECTURE.md`, then `historical/original-assembly/Assembly-Lang/SampleInstructionSet.asm` and the two bridge labs.

## Using Code Composer Studio

Target device **MSP430G2553**. Import the provided `*.projectspec` where present, but verify toolchain syntax for your installed CCS version. For a legacy source that defines `RESET` and `.sect ".reset"`, make an **assembly-only** project; do not combine this with C runtime reset-vector setup.

## One source of truth

The [GitHub curriculum source tree](https://github.com/RandoSY/nugget-physical-computing/tree/main/msp430g2553-learning-labs) is authoritative, including the 30 original assembly files. Local ZIP packages are convenience distributions and should carry checksum/version notices.
