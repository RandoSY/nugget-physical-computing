# Instructor Handbook — From Bits to Behavior, PDP-11 to Edu430

## Mission

Develop confident microprocessor literacy: predict a physical behavior, express it in readable C, peel back Edu430 macros into the exact register operation, examine the equivalent assembly-level state transition, and transfer the model to a historically important machine architecture. Students should leave knowing **how** the machine works, not only how to reuse a function.

## Audience, schedule and prerequisites

A 16-week course, approximately 3 contact hours and 1–2 independent hours per week. Appropriate for introductory undergraduate / advanced secondary STEM with basic variables, loops and arithmetic. Each week supplies a back-row, mainstream and advanced path without splitting the curriculum into permanently different courses.

## Hardware kit

- TI MSP430G2553 LaunchPad (original G2 or newer G2ET), Windows computer and Code Composer Studio; optional oscilloscope/logic analyzer.
- USB backchannel UART at 9600 8-N-1 using P1.1 RX / P1.2 TX; hardware jumper wiring varies by board revision.
- Potentiometer, TMP36 or suitable 3.3V analog temperature sensor, HX711 load cell, HC-SR04 with safe 5V-to-3.3V echo divider, VL6180X breakout rated for 3.3V logic. Optional compatible SPI flash and USB serial terminal.
- Hardware I2C uses P1.6 (SCL) and P1.7 (SDA), 3.3V pull-ups and **LED2 jumper removal**. Hardware I2C and SPI share USCI_B0 and cannot operate simultaneously in two separate modes. ADC and pin mux assignments require careful checking.

## Core teaching cycle

**Predict → Build → Observe → Explain → Transfer.** The same experimental result must be shown in at least two representations. Trace from `EDU_PIN_TOGGLE(P1,BIT0)` to `P1OUT ^= BIT0` to voltage level, then compare an assembly `XOR.B` operation and a PDP-11 `BIC/BIS` bit-operation sequence. Not every PDP-11 operation is exactly equivalent; explaining the difference is graded positively.

## Learning goals

1. Read/write elementary memory-mapped I/O registers and control bit masks.
2. Use GPIO, clocks, timers, interrupts, UART, ADC, PWM, I2C and SPI safely.
3. Explain key assembly addressing modes, branches, register allocation and stack mechanics.
4. Interpret macro expansions and decide appropriately between a macro, inline function and full driver.
5. Build scientific measurements using calibration, repeatability, uncertainty and a serial data log.
6. Implement cooperative time-based tasks without separate task stacks and explain overruns.
7. Compare MSP430 with PDP-11 accurately, identifying both common concepts and crucial ISA differences.

## Classroom setup: deliberately avoid the common traps

- Confirm **which LaunchPad version** is on each bench before programming. Target CPU remains MSP430G2553, but debugging interfaces differ.
- Assembly-only labs: new MSP430G2553 CCS project with **one** `.asm` defining reset/startup; do not combine a historical `RESET` vector with CRT startup or any other `main()` and vector. Original code may need linker compatibility adjustments.
- C labs: one independent `main.c` per CCS project; teach IDE import first as a diagnostic check, then explore functionality.
- MSP430G2553 GPIO is 3.3V territory. The HC-SR04 ECHO line must be level shifted; do not confuse breakout VIN with logic I/O voltage.
- If the sensor is not responding, inspect grounds, SDA/SCL line levels, ACK, pin mux, P1.6 LED2 jumper and VCC before changing firmware.

## Learner differentiation

**Access path (back row):** launch a known working core example, predict one binary behavior, use register watch, add comments explaining the state change. **Standard path:** alter parameter / measurement, explain variable and register dependencies. **Extension path:** generate equivalent assembly, locate subtle flag difference, measure time/power, or refactor driver with explicit error cases.

## Frequent conceptual mistakes and corrective probes

- "Macro means runtime function": inspect preprocessor expansion; identify no-stack simple bit change.
- "The CPU is multitasking": demonstrate polling and timer ISR vs cooperative main loop; identify blocking UART/jitter.
- "All 16-bit CPUs are compatible": distinguish register count, PC/SP placement, flags, branch behavior and encoding.
- "I2C is just four bytes": inspect device addressing, ACK, repeated START, bus arbitration assumptions and register width.
- "Source compiles, so it works": demonstrate host syntax test vs target compiler and electrical hardware test.
- "Original archive is instructional ground truth": identify misleading `CountBinary-1s.asm` comment; distinguish provenance from correctness.

## Assessment system

- Weekly lab notebook (30%): predictions, source and register values, physical observation, failures.
- Two architecture quizzes (15%): operand types, PC/SP mapping, flags and addressing.
- Sensor calibration report (15%): units, range, uncertainty, repeats, known references.
- Interface/scheduler lab (15%): bounded timeouts, I2C bus capture, task latency.
- Final team capstone + individual viva (25%): reproducibility, line-by-line macro expansion, assembler trace, PDP-11 comparison.

## Continuous assessment rubric, 0–4 each

Accuracy of observations; correctness of register/assembly explanation; evidence of measurement and controls; code organization/error handling; clarity and intellectual honesty about unverified tests. Four = reproducible measured evidence and correct explanations; 3 = correct with minor omissions; 2 = mostly working but weak evidence; 1 = incomplete mental model; 0 = absent.

## Suggested extension activities

- Estimate energy per sensor report with G2ET EnergyTrace; compare active loop and LPM0. Do not claim G2553 EnergyTrace++.
- Use VL6180X model-ID, register-pointer transfer and millimeter output as the generic I2C case study; then substitute another simple register sensor without rewriting the transport.
- Trace an assembly stack across CALL and RET, comparing PDP-11's R6 SP with MSP430's R1 SP.
- Use the assembly array routines to test bounds and assumptions, without modifying archived upstream originals.

## Qualification and status

The 30 original assembly sources are **losslessly recovered by Git blob identity**; they have not all been compiled or hardware-verified. Bridge lab sources supplied here are pedagogical additions. Existing C/Edu430 packages have host syntax checks but peripheral bus and physical qualification remains a distinct task. Teachers should retain the original assembly archive and corrections side by side.
