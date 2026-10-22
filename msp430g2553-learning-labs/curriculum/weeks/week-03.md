# Week 03 — Input, decisions and debouncing

**C laboratory:** [`lessons/03-patterns`](../../lessons/03-patterns/README.md)  
**Assembly reading:** [`Exercises/Lab2.asm`](../../historical/original-assembly/Assembly-Lang/Exercises/Lab2.asm)  
**Student evidence:** Poll and debounce; test state transitions  
**Hardware:** Button and two LEDs

## Three passes through the same idea

1. **Observe:** identify what an instrument, pin, bus, timer, or register does. Record a prediction before running it.
2. **Express:** implement or examine the behavior in readable C / `EDU_` macros. Expand one macro into the actual TI register statement.
3. **Explain:** single-step a related MSP430 assembly routine; examine registers, flags, memory and stack. State where a PDP-11 implementation would behave similarly or differently.

## Classroom sequence (approx. 3 contact hours)

- 20 min: instructor demonstration and explicit electrical or architecture safety check.
- 35 min: pair work with a starter program; make a recorded prediction.
- 50 min: debugger observations, engineering notebook evidence, and one controlled variation.
- 25 min: translate one construct between C, macro and assembly views.
- 30 min: verification, comparison, discussion and exit ticket.
- 20 min: extension or instructor office-hours buffer.

## Exit ticket

Explain which register or condition flag is changed, how you know the outcome is correct, and one assumption or failure mode. Include a screenshot, register table, serial log, or measurement with units. If you did not build on the actual board, write **NOT HARDWARE VERIFIED**.

## Differentiate without diluting rigor

**Back-row route:** use the provided runnable C example and trace two named lines with the debugger. **Main route:** change exactly one parameter and predict the response. **Advanced route:** rewrite the controlling expression in assembly or prove equivalence across PDP-11 and MSP430, including differences in addressing modes or flags.

**Safety / grading:** refer to `curriculum/docs/INSTRUCTOR_HANDBOOK.md`. Never power a 5-V echo directly into MSP430 GPIO; disconnect the P1.6 LED jumper during hardware I²C; share GND; use a 3.3-V-compatible sensor board.
