# Bridge 01 — One LED, three views

**Do not compile C and assembly at the same time.** The two sources are separate programs with separate startup paths.

- `main.c`: direct C + explicit Edu430 macro; see pin P1.0 (LED_A). Source-level comparison intentionally tiny.
- `main.asm`: TI assembler reset vector and register writes, manual blink via a delay counter. Requires genuine CCS assembler verification.
- PDP-11 conceptual foil: `BIS #1,PERIPHERAL` could express a memory-mapped bit-set when wired to an appropriate device register; actual PDP-11 register names/addresses and bus systems differ. Do not try to run MSP430 GPIO addresses on a PDP-11.

## Student worksheet

1. Predict after which instruction P1DIR bit 0 becomes 1.
2. Find every write to P1OUT. Draw a timing diagram and record nominal delay loop iterations.
3. Replace `EDU_PIN_TOGGLE(P1,BIT0)` with `P1OUT ^= BIT0` and compare the generated effect.
4. Distinguish the GPIO direction register from the output latch register.
5. State which values cannot be assumed equivalent across two different CPU architectures.
