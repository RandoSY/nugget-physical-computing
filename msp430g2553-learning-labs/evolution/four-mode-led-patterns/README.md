# Four-mode LED patterns: four perspectives on one problem

The teaching target is the 2021 [Joel Brigida LED-pattern exercise](https://github.com/ADolbyB/msp430g2553-labs/blob/main/C-Lang/LEDs/OnBoard-LED-Pattern.c). The original, which remains at the link, uses direct `P1DIR` / `P1OUT` / `P1IN` register operations. This folder is **new instructional code** with the same four observable LED modes; it is not a claim that the snippets below are verbatim copies of the historical program.

1. `01-register-operations`: exact bit twiddling is visible in the application.
2. `02-generic-bit-macros`: generic `SET_BITS`, `CLEAR_BITS`, `TOGGLE_BITS` reveal the operations without making a device abstraction.
3. `03-semantic-actions`: `LED_ON`, `BUTTON_DOWN` and named roles show meaning first, and delegate registers to `edu430.h`.
4. [Lesson 03](../../lessons/03-patterns): separated named state and `display` / `press_event` functions.

**All stages have the same pin assignment** (P1.0 LED A, P1.6 LED B, P1.3 button) and a four-mode cycle: A only, B only, both, alternate. Behavior timing is instructional rather than a cycle-exact reproduction of historical code. Compile each stage as a separate CCS project; never add multiple `main.c` files to one project. Compare the register trace and state diagram across stages. Do not infer LED *colors* from pin names.

### The reveal

| Teaching command | General bit operation | Plain register C |
|---|---|---|
| `LED_ON(LED_A)` | `SET_BITS(P1OUT,BIT0)` | `P1OUT |= BIT0` |
| `LED_OFF(LED_A)` | `CLEAR_BITS(P1OUT,BIT0)` | `P1OUT &= ~BIT0` |
| `LED_TOGGLE(LED_B)` | `TOGGLE_BITS(P1OUT,BIT6)` | `P1OUT ^= BIT6` |
| `BUTTON_DOWN()` | `BITS_LOW(P1IN,BIT3)` | `(P1IN & BIT3) == 0` |

Discuss what C preprocessor macros **do not** do: type checking, run-time delays, or automatically configuring ports. This is scaffolding that can be removed, not a hidden framework.
