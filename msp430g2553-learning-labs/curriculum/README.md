# Integrated 16-Week MSP430G2553 / Edu430 / Assembly Curriculum

**From Bits to Behavior — and Back to the PDP-11.**

The complete course is a teaching SPINE, not a new implementation replacing the existing labs. The eleven C lessons, four inquiry appendices, the optional Edu430 library, and the thirty upstream assembly programs retain their identities.

| Week | Concept | C anchor | Original assembly anchor | Evidence of learning |
|---:|---|---|---|---|
| 1 | Observe, power, and bit-level thinking | `lessons/01-blink` | `SimpleVariables.asm` | Watch a GPIO bit change; inspect P1DIR/P1OUT |
| 2 | A machine built from registers | `lessons/02-button` | `SampleInstructionSet.asm` | Trace MOV, BIS, BIC; compare PDP-11 register naming |
| 3 | Input, decisions and debouncing | `lessons/03-patterns` | `Exercises/Lab2.asm` | Poll and debounce; test state transitions |
| 4 | Arithmetic, flags and conditional branches | `lessons/05-interrupt` | `Count-Binary-1s/CountBinary-1s.asm` | Build a flags truth table; compare C and assembly |
| 5 | Loops, shifts and algorithm equivalence | `lessons/03-patterns` | `Multiplier/MultiplierShiftAdd.asm` | Implement shift-add multiplication in C and ASM |
| 6 | Addresses, arrays and memory | `lessons/10-telemetry` | `Array-Functions/FindMinArrayIndex.asm` | Trace direct, indirect, indexed and autoincrement |
| 7 | Timers, interrupts and real time | `lessons/04-timer` | `Flip-Bits-R5-R6/FlipBits-R5-R6.asm` | Measure timer interrupt timing; study status register |
| 8 | PWM, clocks and power | `lessons/06-pwm` | `SampleInstructionSet.asm` | Duty cycle and EnergyTrace comparison |
| 9 | Stacks, calls and subroutines | `lessons/08-uart-transmit` | `N-Factorial/CalcN-Factorial.asm` | Stack snapshots at CALL and RET; compare PDP-11 |
| 10 | ADC and numerical measurement | `lessons/07-adc` | `Exercises/MathInAssy-v1.asm` | Report ADC counts and volts with assumptions |
| 11 | Serial protocol as scientific instrument | `lessons/09-uart-commands` | `Sequence-Detection/SequenceDetection-Final.asm` | Design a command and data reporting grammar |
| 12 | SPI and I2C: reusable buses | `lessons/08-uart-transmit` | `Exercises/Lab3-v1.asm` | Identify START, address, ACK and repeated START |
| 13 | Optical time-of-flight exemplar | `lessons/10-telemetry` | `Sequence-Detection/SequenceDetectionBytes.asm` | Read VL6180X model ID, initialize and measure |
| 14 | Measurement science and calibration | `lessons/10-telemetry` | `Array-Functions/AddTwoArrays.asm` | Compare scale, temp and ultrasonic uncertainty |
| 15 | Cooperative concurrency and low power | `lessons/11-tiny-multitasker` | `Exercises/LabFinal.asm` | Measure jitter, latency, EnergyTrace where available |
| 16 | PDP-11 comparative capstone | `lessons/11-tiny-multitasker` | `SampleInstructionSet.asm` | Translate machine behaviors, present a documented instrument |

## How to navigate

- [Instructor handbook](docs/INSTRUCTOR_HANDBOOK.md) — objectives, classroom procedures, assessments, differentiation.
- [PDP-11 architecture comparison](docs/PDP11_ARCHITECTURE.md) — factually grounded parallels and limits.
- [Assembly recovery catalog](docs/ASSEMBLY_PROVENANCE.md) — 30 unchanged files, license, integrity, and original links.
- [Bridge Lab 01](bridge-labs/01-bits-and-registers/README.md) — same observable GPIO operation in C, EDU macros and assembly.
- [Bridge Lab 02](bridge-labs/02-count-ones/README.md) — count set bits in C and MSP430 assembly, plus PDP-11 comparison.
- [Scientific instrument capstone](docs/CAPSTONE.md) — measurement, calibration, serial export, timing, bus use and trace evidence.

## Preservation guarantee

- `historical/original-assembly/Assembly-Lang/`: all thirty files restored as exact upstream Git blobs in the published **GitHub repository**. **Do not treat the originals as modernized or qualified programs.**
- `lessons/`: first eleven lessons remain unchanged; do not combine multiple `main.c` files or reset-vector definitions in one CCS target.
- `appendices/`: A TMP36, B HX711, C HC-SR04, D software-I²C VL6180X.
- `fwlib/`: Edu430 v0.2 framework, optional v0.3 16-bit-register VL6180X hardware-I²C exemplar, eight general examples and an optical exemplar.

## Compiling in Code Composer Studio

Select **MSP430G2553**, TI MSP430 compiler/assembler, and **one source entry point per project**. The historical assembly files include their own reset vector and are best imported as separate **assembly-only projects**, one file at a time. Modern CCS may require file-specific build adjustments. Do not drop an `.asm` file into an existing C lab with its own startup/reset vector. Confirm LaunchPad target and debugger revision independently.

## Verification boundaries

Original assembly integrity is verified from Git blob hashes. That is not an assembly build or semantic test. Existing C / Edu430 host mock checks establish syntax only; course measurements and USCI bus drivers require the MSP430 compiler and real hardware. Our two bridge-lab examples are educational additions and must likewise be qualified on the board.

## Source attribution

Original `.asm` files: Joel Brigida / ADolbyB, [msp430g2553-labs](https://github.com/ADolbyB/msp430g2553-labs), GPL-3.0. Edu430 and curricular adaptations are GPL-3.0. Piconomix PX-FwLib (MIT) is an architectural inspiration, not copied source. Digital Equipment PDP-11 manuals are cited for comparison, not bundled.
