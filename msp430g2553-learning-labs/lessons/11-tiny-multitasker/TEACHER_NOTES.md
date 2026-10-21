# Teacher Guide — Lesson 11: A Tiny Cooperative Multitasker

**Suggested duration:** Two 50–60 minute periods + optional EnergyTrace extension. **Prerequisites:** Lessons 1–5 and 8–9 (GPIO, button, Timer_A ISR, UART). **Hardware:** MSP430G2553 in MSP-EXP430G2 or MSP-EXP430G2ET; USB cable; PC UART terminal; G2ET required for onboard EnergyTrace.

## Essential question

**How can one CPU appear to do several things at once without an RTOS?** Students build observable evidence: heartbeat continues while presses are detected and telemetry is sent.

## Outcomes (assess separately)

1. Explain the difference between a periodic hardware interrupt and an ordinary C task.
2. Predict lost events caused by blocking `__delay_cycles()`; contrast with 10ms polling.
3. Calculate a 1ms Timer_A interval from a calibrated 1MHz clock and `TA0CCR0=999`.
4. Trace `RUN_EVERY(led_job, now, blink_led)` through its `if` expansion to `P1OUT ^= BIT0`.
5. Explain `volatile`, rollover-safe uint16 timing, simple debouncing, and why long tasks delay others.
6. Form and test a prediction about LPM0-versus-active energy use on G2ET; communicate units, controls, and limits.

## Teaching sequence

**Engage (10 min):** Run blocking stage. Ask students to make very quick button taps during the quarter-second wait. Predict whether each press will be reported; record actual counts. Note that stage 1 samples the button only once per delay, without debounce.

**Explore (20 min):** Run Stage 2. Examine `TA0CCR0=999` and `TIMER0_A0_VECTOR`. Show that ISR increments `ticks_ms` and returns; main compares 250/10/1000ms deadlines. Ask why no `__delay_cycles()` appears in any job. Discuss the serial helper's much shorter blocking time.

**Explain (20 min):** Open Stage 3's `tiny_scheduler.h`. Expand `RUN_EVERY` on the whiteboard and trace `job_is_due()` and the `blink_led()` body. Explain the `do { } while (0)` macro idiom and ask where CPU context switches would occur. Correct answer: nowhere; all jobs share one call stack.

**Elaborate (20 min):** Ask teams to change the heartbeat to 100ms while preserving the button's 10ms interval. Have them briefly insert a 1s blocking delay in `report_events()`, then explain jitter/lost samples. Remove it afterward. Challenge: rework an HC-SR04 measurement as a nonblocking state machine using Timer_A capture or a separate timer.

**Evaluate (10 min):** Students submit a short timing diagram, a macro-to-register expansion, button detection counts, and a paragraph on cooperative vs preemptive scheduling.

## Prediction worksheet / experiment log

| Condition | Predicted detected presses | Actual detected presses | Why? |
| --- | --- | --- | --- |
| Stage 1: ten fast taps | | | |
| Stage 2: ten fast taps | | | |
| Stage 3: ten fast taps | | | |
| Stage 3: hold button 3 seconds | | | |
| Stage 3: insert slow task | | | |

Control the pace of taps, duration, and stage firmware; do not claim exact button response bounds without measuring actual timing.

## EnergyTrace optional investigation (G2ET)

**Question:** Does a cooperative scheduler use less energy when it sleeps between 1ms timer ticks? Use the **same compiled source** except for `IDLE_LOW_POWER` set to `0` or `1`.

**Independent variable:** active spin vs `LPM0` idle. **Controls:** supply, board, debugger, clock calibration, task periods, LEDs (default off), UART (default off), measurement capture duration, source revision, environmental conditions. **Dependent variables:** mean current (A or uA), electrical power (W), and total energy (J or uJ) reported by CCS EnergyTrace.

1. Verify measurement is from **target MCU**, not from the entire PC or external apparatus. Remove or disable other load paths according to TI board documentation.
2. Compile, flash and run Stage 4 with `IDLE_LOW_POWER=0`, `POWER_TEST_LED=0`, `POWER_TEST_UART=0`. Use CCS EnergyTrace capture long enough to average multiple scheduler cycles. Export values.
3. Only change `IDLE_LOW_POWER=1`; rebuild, run, capture the same duration. Timer interrupts continue in LPM0 because SMCLK remains active, and ISR clears LPM0 on exit.
4. Repeat both conditions, alternate order to reduce drift, and report mean and variability. Calculate `relative_change = 100 * (I_active - I_LPM0) / I_active` **using measured values only**.
5. Optionally set `POWER_TEST_LED=1` for both tests. Ask why the LED can dominate the total power and obscure the CPU improvement.

**Cautions:** G2553 permits EnergyTrace current/energy, **not EnergyTrace++ CPU-state traces**. The 1ms interrupt cadence costs energy. Debugger breakpoints and UART activity distort measurement. LPM3 would stop SMCLK and is **not** a drop-in substitute.

## Common misconceptions

- `RUN_EVERY` runs jobs simultaneously. **No**: it chooses which task to call at each pass through `main()`.
- `__delay_cycles(250000)` always equals 250ms. **No**: this depends on MCLK configuration.
- `volatile` makes a multi-byte variable atomic. **No**: this example deliberately uses a native 16-bit aligned tick.
- `now - last` is broken when time wraps. **No**: cast to unsigned `uint16_t` ensures modulo arithmetic for frequently serviced periodic jobs.
- An ISR should print a serial report. **No**: serial output is slower and is kept out of the high-rate ISR.
- A simpler macro improves timing by itself. **No**: its semantics are the same as the expanded C; task execution time matters.
- Any low-power mode works. **No**: this lab needs SMCLK, so LPM0 is appropriate.

## Grading (20 points)

- 4: predicted vs observed button event investigation with controlled trials.
- 4: annotated Timer_A clock calculation and timeline.
- 4: correct verbatim expansion of `RUN_EVERY` and equivalent GPIO register statement.
- 4: explanation of cooperative blocking, scheduler jitter, and rollover assumptions.
- 4: design improvement or valid EnergyTrace experiment with units, controls, repeats, and limitations.

## Classroom acceptance / validation status

- Host GCC syntax checks and deterministic state-machine tests can be run off-target; these **do not substitute for CCS/MSP430 compiler or physical hardware tests**.
- Before handing to students, build all stages against the MSP430G2553 compiler/device headers, check flash and RAM usage, exercise UART and GPIO on both boards, and compare EnergyTrace on a G2ET.
- Board-specific support/legacy debugger issues are detailed in the course-wide setup guide.
