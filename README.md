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
