<img src="assets/logo.svg" width="520" alt="MSP430 Learning Labs: From Bits to Behavior" />

# MSP430G2553 Learning Labs — From Bits to Behavior

**Eleven carefully staged bare-metal C lessons (including a cooperative multitasking extension), four inquiry STEM measurement appendices, and an extensively documented teacher's guide.** Runs on the MSP430G2553 MCU in the old **MSP-EXP430G2** and newer **MSP-EXP430G2ET** LaunchPads, subject to differing legacy USB debugger support and jumper settings.

<img src="assets/progression.svg" width="960" alt="Each beginner-friendly command expands to familiar C register operations" />

## The educational strategy: let the source code gestate

1. **Observe** a real thing (LED, button, PWM waveform, serial reading, sensor value).
2. **Name** the thing using beginner-friendly code: `LED_ON(LED_A);`.
3. **Reveal** the simple general-purpose macro: `SET_BITS(P1OUT, BIT0);`.
4. **Expand** it into the exact hardware statement: `P1OUT |= BIT0;`.
5. **Investigate** which register is read or written and why the bit mask matters.
6. **Refactor** wisely: retain macros for one-line bit manipulations, use ordinary C functions for stateful hardware operations and safety checks.
7. **Transfer** the idea to another MCU, keeping conceptual intent while changing the register implementation.

The macros are not a proprietary runtime and do not hide the bare metal. They are teaching scaffolding that can be progressively removed. The learner sees the complete path from meaning to mechanism.

## Four stages of the same LED experiment

[Study the progression](evolution/four-mode-led-patterns/README.md): register statements → generic bit macros → named actions → separated functional design. This is the central pedagogical artifact, not just a renamed source file. Run each version and compare actual GPIO behavior.

## Lessons

| Lab | Focus | Hardware |
| --- | --- | --- |
| [01 Blink](lessons/01-blink) | Direction and bit masks | LED A |
| [02 Button](lessons/02-button) | Inputs, pull-ups, debounce | Button + LED |
| [03 Patterns](lessons/03-patterns) | Modes, branches, state | LEDs + button |
| [04 Timer](lessons/04-timer) | Timer_A ISR and timebase | LED |
| [05 Interrupt](lessons/05-interrupt) | Port 1 interrupt | Button + LEDs |
| [06 PWM](lessons/06-pwm) | Hardware duty cycle | LED B |
| [07 ADC](lessons/07-adc) | Analog data | 3.3 V potentiometer |
| [08 UART TX](lessons/08-uart-transmit) | Serial output at 9600 baud | USB virtual COM |
| [09 UART RX](lessons/09-uart-commands) | Receive commands | USB virtual COM |
| [10 Telemetry](lessons/10-telemetry) | CSV-based instrument | ADC + button + UART |
| [11 Tiny Multitasker](lessons/11-tiny-multitasker) | Blocking -> timed jobs -> macro scheduler -> LPM0 | Onboard LEDs, button, UART; optional EnergyTrace |

**Inquiry STEM appendices:** [A TMP36 temperature](appendices/A-temperature), [B HX711 scale](appendices/B-hx711-scale), [C HC-SR04 ultrasonic](appendices/C-ultrasonic), and [D VL6180X optical ToF + software-I2C](appendices/D-vl6180x-software-i2c) (advanced experimental).

## Teacher assets and setup

- [Teacher's Guide (PDF)](docs/Teachers_Guide.pdf) (included in downloadable ZIP; PDF not necessarily tracked in GitHub).
- [Teacher's Guide text](docs/TEACHERS_GUIDE.md), [assessment rubric](docs/ASSESSMENT_RUBRIC.md).
- [CCS 21 setup](docs/CCS_SETUP.md), [G2 vs G2ET compatibility](docs/BOARD_COMPATIBILITY.md), [sensors/wiring/calibration](docs/SENSOR_WIRING_AND_CALIBRATION.md).
- Each lesson folder has a fully standalone `main.c` and an identical project-local `edu430.h`. You need only these two files in a fresh **MSP430G2553** CCS C project. This deliberately avoids incompatible legacy Eclipse project metadata.

## Serial and board-specific details

Use the built-in USB backchannel UART: **P1.1 RXD / P1.2 TXD, 9600 baud, 8N1**, configure the jumpers appropriately on the older board, and open the Windows virtual COM port. The newer G2ET uses eZ-FET; the older G2 debugger may not be supported by recent CCS configurations even though the target firmware is compatible. See the board guide.

**Electrical safety:** MSP430 GPIO uses ~3.3 V; NEVER apply the HC-SR04's 5 V echo directly. The ultrasonic appendix provides a divider. For VL6180X, use a 3.3 V I/O compatible breakout and check its regulator. The HX711 example requires calibration before gram values are meaningful.

## Teaching and verification status

- All programs are provided as commented C sources for MSP430G2553 device headers. Lesson 11 adds four documented stages, CCS ProjectSpec import candidates, a diagram and scheduler logic tests. Host syntax validation sources are included in `tools/`; genuine CCS/board qualification is still pending. See [validation checklist](docs/VALIDATION_AND_HARDWARE_CHECKLIST.md).
- There are 21/21 automated host-side C syntax checks with a simulated MSP430 header, plus passing Lesson 11 deterministic logic tests; no actual target compiler or physical board was available during package creation.
- Core lessons 1–10 are **source-ready**; Lesson 11 includes four TI `.projectspec` project-import templates which require CCS verification. The full package is **not hardware-certified**. Before teaching, create a project in your installed CCS 21, compile each lab, and bench-verify both boards and every external sensor.
- The core code is designed for a calibrated 1 MHz DCO and 512 bytes of RAM.

## Origin, attribution and license

Educational modernization inspired by [Joel Brigida / ADolbyB MSP430G2553 labs](https://github.com/ADolbyB/msp430g2553-labs) (notably `C-Lang/LEDs/OnBoard-LED-Pattern.c`, 2021), which is GPLv3. This adaptation is GPL-3.0; see [sources and attribution](docs/SOURCES_AND_LICENSE.md). Independent from TI and ST. Preserve source lineage while making original bit-register behavior teachable.


## Optional Edu430 FwLib — Piconomix-inspired macro atlas (v0.2)

**[Explore Edu430 FwLib](fwlib/README.md)**: a 14-header, no-RTOS support layer
for GPIO, pull-ups, interrupts, Timer_A, PWM, ADC10, Comparator_A+, clock,
watchdog, UART, SPI, both hardware and software I²C, button debounce,
and periodic jobs.  The API always explains the corresponding raw registers.

This optional expansion does **not modify or replace Lessons 1–11**.
The new examples each ship with a self-contained CCS `.projectspec` candidate
and local header copies.  See [Macro Atlas](fwlib/docs/MACRO_ATLAS.md),
[CCS guide](fwlib/docs/CCS_GUIDE.md), and [Historical origins](fwlib/docs/ORIGINS.md).

The library's host syntax tests are separate from the prior lesson tests;
UART/SPI/I²C require bench verification and proper 3.3 V pin wiring.
[Full curriculum + FwLib ZIP](downloads/MSP430G2553_Learning_Labs_11_Lessons_4_Appendices_Edu430_FwLib.zip)
and [FwLib standalone ZIP](downloads/MSP430G2553_Edu430_FwLib_v0_2.zip) are available.


## Hardware I2C and ST optical ToF exemplar

An optional [VL6180X hardware-I2C exemplar](fwlib/examples/09-vl6180x-hardware-i2c/README.md) extends the library with a general repeated-START bus transaction and **16-bit register addressing**. It retains the existing [software-I2C optical appendix](appendices/D-vl6180x-software-i2c/README.md) as an alternative for comparison. See [teacher notes](fwlib/docs/ST_VL6180X_EXEMPLAR.md). Hardware qualification remains required.


## Integrated assembly + PDP-11 curriculum

[**Start the unified 16-week course here**](curriculum/README.md). The course connects all eleven C lessons, four inquiry STEM appendices, Edu430 register/macro drivers, cooperative scheduling and the recovered **30 upstream MSP430 assembly examples**. It adds two annotated C/assembly bridge labs, PDP-11 ISA comparisons, weekly plans, assessment and an instructor handbook.

The preserved originals are at [historical/original-assembly/Assembly-Lang](historical/original-assembly/Assembly-Lang) in the GitHub release; 30/30 blobs matched upstream. Differentiate unchanged archival sources from the new student bridge code. The GitHub branch ZIP provides the complete recovered assembly set.
