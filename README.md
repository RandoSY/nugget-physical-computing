# Nugget Physical Computing

Nugget is the low-cost physical-computing learning ladder: a progression that makes computation increasingly visible before hiding it behind convenience.

## Learning ladder

A representative sequence is:

1. **555 timer + 7400-series logic** — timing, state, gates, visible signals.
2. **Simple processor simulation** — instruction execution and machine state made observable.
3. **PIC / AVR 8-bit microcontrollers** — small systems close to the hardware.
4. **Arduino Uno + Multi-Function Shield** — fast access to buttons, LEDs, display, buzzer, ADC, and serial interaction.
5. **RP2040 instrumentation** — logic-analyzer/scope-style work and programmable I/O.
6. **Pico / Pico W / ESP32-class systems** — richer connected physical computing when the need is established.

The exact board is secondary. The durable asset is the progression from understandable mechanism to useful abstraction.

## Design principles

- Start with behavior the learner can see.
- Name examples by what they do.
- Keep the smallest runnable example small.
- Introduce libraries only when they buy real capability.
- Preserve command-line and low-level paths where they teach something useful.
- Prefer inexpensive hardware that a learner can own and repeat experiments with.

## Related work

Nugget connects naturally to:

- `nugget-fwlib` and low-level firmware exercises;
- PIC, AVR, CH32, RP2040, and Arduino examples;
- Forth/processor observatories in [computing-observatories](https://github.com/RandoSY/computing-observatories);
- programmers and debugging infrastructure in [embedded-tools](https://github.com/RandoSY/embedded-tools);
- SDL endpoints in [software-defined-laboratory](https://github.com/RandoSY/software-defined-laboratory).

## Planned repository structure

- `00-foundations/`
- `10-processor-models/`
- `20-pic-avr/`
- `30-arduino-mfs/`
- `40-rp2040/`
- `50-connected-systems/`
- `docs/`
- `validation/`

## Current state

**Lifecycle:** `active`

This repository currently defines the canonical learning progression. Existing code and historical examples still need to be curated into this structure, with each migrated example labeled as verified, upstream, historical, or not yet reproduced.


## MSP430G2553 — From Bits to Behavior (10-lesson course)

The [MSP430G2553 Learning Labs](msp430g2553-learning-labs/README.md) curriculum modernizes register-level LaunchPad examples into a progressive learning sequence: direct registers → generic bit macros → readable hardware commands → small C functions. It includes ten labs, four inquiry-measurement appendices (TMP36, HX711, HC-SR04, VL6180X), a teacher's guide, and wiring guidance for both G2 and G2ET LaunchPads. [Download the corrected complete ZIP](msp430g2553-learning-labs/downloads/MSP430G2553_Learning_Labs_10_Lessons_4_Appendices.zip). **17/17 host syntax tests passed; physical and CCS compiler tests still needed.**


### Lesson 11: MSP430 Tiny Multitasker

The [11-lesson MSP430G2553 Learning Labs curriculum](msp430g2553-learning-labs/README.md) now includes [Lesson 11: A Multitasker You Can Understand](msp430g2553-learning-labs/lessons/11-tiny-multitasker/README.md): blocking C -> explicit Timer_A scheduling -> `RUN_EVERY` macro and a fully inspectable elapsed-time check -> optional LPM0/EnergyTrace comparison. Four stage-specific CCS ProjectSpec templates, an expanded Teacher's Guide, and a [new full-curriculum ZIP](msp430g2553-learning-labs/downloads/MSP430G2553_Learning_Labs_11_Lessons_4_Appendices.zip) are included. Host checks passed (21/21 source syntax, 10/10 logic tests); MSP430 compiler builds, project imports and physical measurements remain to be verified.


### Edu430 FwLib v0.2 — Piconomix-inspired macro library

The [MSP430G2553 course's optional Edu430 FwLib](msp430g2553-learning-labs/fwlib/README.md) extends the *From Bits to Behavior* teaching ladder to GPIO, interrupts, PWM, ADC10, Timer_A, UART, SPI, **both hardware and software I²C**, and the Lesson 11 cooperative scheduler. Includes eight CCS ProjectSpec source examples, an [annotated macro atlas](msp430g2553-learning-labs/fwlib/docs/MACRO_ATLAS.md), and a [standalone download](msp430g2553-learning-labs/downloads/MSP430G2553_Edu430_FwLib_v0_2.zip). Existing 11 lessons are preserved; new bus drivers require CCS/real hardware qualification.


### Edu430 expert manual

[**Edu430 FwLib v0.2 — User / Programmer / Instructor Guide (PDF, 28 pages)**](msp430g2553-learning-labs/fwlib/docs/Edu430_FwLib_v0_2_User_Programmer_Instructor_Guide.pdf). Complete macro/inline-function reference, CCS quick starts, board resource conflicts, eight STEM teaching sessions and honest qualification notes.
