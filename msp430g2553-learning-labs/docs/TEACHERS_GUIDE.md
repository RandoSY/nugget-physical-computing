# Teacher's Guide — MSP430G2553 Learning Labs

**From Bits to Behavior | Ten lessons + four inquiry STEM appendices**

Audience: beginning embedded C / physical computing students, grades 10-12 to college.
Recommended duration: ten 60-90 minute meetings, plus inquiry work and hardware checkout.

## Teaching thesis: a revealing abstraction

Students should not be forced to master bitwise operators before they can make
an observable prediction. Begin with an expressive command (`LED_ON(LED_A)`),
then translate it into a generic macro (`SET_BITS(P1OUT, BIT0)`), and finally
reveal the underlying plain register C (`P1OUT |= BIT0`). This is a deliberate
**progression of disclosure**, not an attempt to bury the hardware in libraries.

Keep a register translation sheet visible. Insist on a hardware-level account
of each operation by the end of the lab. Compare the historic Joel Brigida
LED example at its original URL, observing how a large loop becomes a small
set of named responsibilities. The historical original is not presented as
broken; it is a valuable bridge to an earlier pedagogical style.

## Hardware and safety

- MCU: **MSP430G2553**, installed in older MSP-EXP430G2 or newer MSP-EXP430G2ET.
- Newer board: eZ-FET, recommended for CCS 21, USB UART virtual COM port.
- Older board: older on-board eZ430 interface; modern IDE support varies.
  Use a supported external MSP430 programmer if necessary. The device-targeted
  C does not change. The UART header jumpers must match hardware mode.
- LEDs P1.0 and P1.6, button P1.3, 9600-baud UART P1.1/P1.2.
- Operating GPIO signals must be at **3.3 V**, never 5 V. All external
  powered circuits need a common ground. Confirm polarity and pin positions
  with the breakout and board documentation before turning power on.
- The ADC potentiometer runs between 3.3 V and GND with its wiper at P1.4.
- Appendices require components and safe level translation noted separately.

## General lesson routine (60-90 minutes)

1. **Predict (10 min):** Draw the signal or value you expect. Identify units.
2. **Inspect (10 min):** Find the friendly macro in source and translate it.
3. **Build (15 min):** Create an MSP430G2553 project in CCS 21 and include
   just that lesson's `main.c` and `edu430.h`.
4. **Observe (15 min):** Flash, debug, collect evidence, and inspect registers.
5. **Investigate (15-25 min):** Change exactly one variable and predict result.
6. **Explain (5-10 min):** Explain what bit in which register caused the effect.

## Ten lessons at a glance

| No. | Observable behavior | Conceptual focus | Main assessment |
| --- | --- | --- | --- |
| 01 | Blink a light | output direction, setting bits | expand three GPIO macros |
| 02 | Button controls light | active-low, pull-up, debouncing | show P1IN and P1REN |
| 03 | Four modes | state, branching, refactoring | state-transition table |
| 04 | Periodic timing | Timer_A ISR, volatile, rollover | identify tick rate |
| 05 | Button event | interrupt flag, edge, ISR | compare polling vs interrupt |
| 06 | Fade LED | PWM frequency, duty cycle | calculate duty percentage |
| 07 | Two analog thresholds | 10-bit ADC, thresholding | report raw ADC vs volts |
| 08 | Serial sample counter | TX register, 9600 baud | capture five UART lines |
| 09 | Remote LED command | RX register, human interface | command test matrix |
| 10 | CSV instrument | measurement + state + reporting | publish repeatable data |

## Student deliverables for every lesson

One hypothesis, wiring/pin diagram, source change with comments, log or photo
of observable outcome, register-level translation, and a short paragraph on
limitations. For sensors, add a calibration table and estimate uncertainty.

### Lesson 01 — Blink

**Driving question:** What tells the chip that P1.0 is an output?

**Before flashing:** Predict the LED waveform as high/low sections of equal duration.

**Inquiry extension:** Change 250 ms to 100 ms; compare predicted and observed cycle time.

**Teacher explanation:** P1DIR selects output; P1OUT sets latch; |= only changes specified bits.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 02 — Button

**Driving question:** Why does pressing a button read zero?

**Before flashing:** Trace a pull-up resistor and a switch to ground.

**Inquiry extension:** Remove the 20 ms delay, evaluate bounce on ten slow presses.

**Teacher explanation:** P1REN enables resistor; P1OUT chooses pull-up; P1IN samples logic.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 03 — Patterns

**Driving question:** Is a button press an event or a level?

**Before flashing:** Predict each of four mode transitions and two output phases.

**Inquiry extension:** Reverse the mode order and keep the same hardware interface.

**Teacher explanation:** State is stored in a C variable, GPIO registers express that state.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 04 — Timer

**Driving question:** Why can a hardware timer be more reliable than counted delays?

**Before flashing:** Calculate 1000 cycles/tick at 1 MHz and 500 ticks/half-cycle.

**Inquiry extension:** Change 500 ticks to 250 and measure LED timing.

**Teacher explanation:** CCR0 sets period, CCIE enables interrupt, TA0CTL selects clock and mode.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 05 — Interrupt

**Driving question:** What is the purpose of an interrupt flag?

**Before flashing:** Predict changes when the ISR increments presses.

**Inquiry extension:** Explain what happens if P1IFG is never cleared; make timing diagram.

**Teacher explanation:** IE enables; IES selects edge; IFG flags; ISR acknowledges event.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 06 — PWM

**Driving question:** Is brightness caused by voltage or switching duty cycle?

**Before flashing:** Sketch high/low waveform for CCR1=250 out of CCR0=999.

**Inquiry extension:** Measure with DMM or logic analyzer if available; vary duty.

**Teacher explanation:** P1SEL routes a peripheral; CCTL1 sets PWM output mode.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 07 — ADC

**Driving question:** How are 10 bits related to a voltage?

**Before flashing:** Predict raw ADC for one-half of VCC and corresponding LED states.

**Inquiry extension:** Collect readings at five pot settings; plot reading vs position.

**Teacher explanation:** ADC10AE0 routes analog; ADC10MEM supplies sample counts.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 08 — UART transmit

**Driving question:** Why is UART not the same as USB?

**Before flashing:** Identify MCU UART TX, bridge chip, USB COM, terminal.

**Inquiry extension:** Collect a CSV file of ten samples at 9600 8N1.

**Teacher explanation:** USCI_A0 sends bytes; baud divisor matches configured clock.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 09 — UART commands

**Driving question:** What changes when the PC sends input?

**Before flashing:** Make a command table of A,B,O,?.

**Inquiry extension:** Add one new single-letter command and explain its effect.

**Teacher explanation:** UCA0RXBUF exposes received byte; no proprietary protocol.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

### Lesson 10 — Telemetry capstone

**Driving question:** When is a number a meaningful measurement?

**Before flashing:** Specify columns, sampling frequency, trial definition.

**Inquiry extension:** Export data to spreadsheet; compare ten readings at each setting.

**Teacher explanation:** Integrates state, ADC, reports; report assumptions and uncertainty.

**Check:** Ask the learner to expand one macro into the exact register statement, point to the physical pin, and report one observed result.

## Measurement appendices: the inquiry STEM bridge

**A — TMP36 temperature.** Independent variable: bath/environment temperature;
dependent variable: ADC output and computed Celsius. Collect repeated readings
next to a reference thermometer, graph temperature error. Discuss assumed
3.300 V reference and calibration bias. TMP36 is analog, not a thermocouple.

**B — HX711 strain-gauge scale.** Independent variable: known test mass;
dependent variable: signed 24-bit bridge-amplifier output. Record unloaded
baseline (tare), two known masses, slope in counts/gram, and repeated residuals.
Discuss hysteresis, off-axis forces, amplifier noise and drift. The stock
COUNTS_PER_GRAM constant is a **placeholder** and not a real calibration.

**C — HC-SR04 ultrasonic distance.** Independent variable: known target distance;
dependent variable: echo pulse width. Sweep 5-80 cm, graph errors and calculate
speed-of-sound sensitivity to temperature. Sensor uses 5V but MCU echo pin
must see no more than 3.3V; install a divider/level-shifter before first test.

**D — ST VL6180X optical ToF (optional advanced).** Short-range laser/optical
ranging, distinct from an STM32 MCU. Write software I2C with open-drain GPIO,
16-bit register addresses and initialization sequence. Study material
reflectance and near-field calibration; this experimental lab requires physical
verification. A VL53L0X module requires a different driver.

## Sample data and analysis prompts

Log trial number, reference reading, measured value, unit, residual and notes.
For each appendix: distinguish resolution from accuracy; record environmental
conditions; compute average, sample variation and bias; document a failed or
out-of-range measurement without silently converting it to zero.

## Troubleshooting and demonstration rubric

If a light does not blink: check USB power, correct MSP430G2553 device target,
board-specific LED polarity/color, installed code image, and clock calibration.
If UART is silent: inspect hardware-UART jumpers, USCI P1SEL/P1SEL2,
9600/8N1, proper COM port, TX/RX pairing and driver support.
If timer seems wrong: measure SMCLK or verify factory 1 MHz DCO values.
If ADC is saturated: measure the actual wiper voltage, confirm common ground,
and check P1.4 analog input selection. For sensor mismatch: document module
part number and interface voltage, then revisit pinout.

**Assessment (100 points)**: prediction and experimental design (15), electrical
safety (15), functional program (20), abstraction literacy (20), measurement
and calibration (15), documentation/reproducibility (15).

## Instructor validation checklist

The materials have host-side C syntax checks using a fake target header, but
have **not** been compiled with a real MSP430 toolchain or run on either board.
Before student use, each instructor must compile in CCS 21 for MSP430G2553,
flash the exact boards used, verify serial jumpers on the old model, photograph
pin connections, check ADC scaling and verify external sensor examples.

## Provenance and redistribution

Based conceptually on the publicly available Joel Brigida / ADolbyB
MSP430G2553 laboratory collection. Original LED pattern C:
https://github.com/ADolbyB/msp430g2553-labs/blob/main/C-Lang/LEDs/OnBoard-LED-Pattern.c

These teaching sources are distributed under GPL-3.0-or-later with
upstream attribution, and TI/ST reference links in `SOURCES_AND_LICENSE.md`.

## A four-stage comparison workshop

Use [`evolution/four-mode-led-patterns`](../evolution/four-mode-led-patterns) as a dedicated comparison before or after Lesson 03. Students build a register-only stage, replace basic register masks with generic macros, and then substitute semantic commands. Finally they study a version with small named functions. Ask each team to predict which compiled instructions or GPIO waveforms should change (generally not much) and which properties improve (readability, maintainability, conceptual transfer). The evaluation should include an explanation of each bitwise operator in the original code, not just correct LED blinking.

**Handout questions:** What is preserved across all four versions? When does a generic bit macro make code harder to understand than a descriptive function? Why should timer/ADC interactions be wrapped as functions instead of enormous macros? How does the button release wait constrain responsiveness?

## Status and safety for first classroom deployment

The host mock-header syntax test is a smoke test, **not** MSP430 target compilation or hardware validation. Before teaching, build one project per laboratory in a local CCS installation and test each LaunchPad jumper position and accessory breakout. Record actual toolchain version, board revision, observed COM number, test conditions, and any source changes. The optical I2C experiment needs particularly careful device/module qualification.


---

## New Lesson 11: Cooperative Multitasking (optional two-session extension)

The [Lesson 11 Teacher Notes](../lessons/11-tiny-multitasker/TEACHER_NOTES.md) contain the complete teaching sequence, observation worksheet, macro-expansion exercise, timing hypotheses, and EnergyTrace protocol.

**Teacher priorities:** (1) Compare short button taps lost by the blocking example with the 10ms sampler. (2) Demonstrate the Timer_A ISR only increments `ticks_ms`; the CPU performs LED, debounce, and UART tasks in its ordinary main loop. (3) Have students expand `RUN_EVERY(job, now, task)` into its function and `if` statements and end at `P1OUT ^= BIT0`. (4) On G2ET, run EnergyTrace with identical task load and change only `IDLE_LOW_POWER` to test active vs LPM0.

**Lesson 11 validation:** All four new C entry points pass host GCC syntax checks, and scheduler tests cover tick rollover, task overdue handling, and button events. CCS/board hardware validation and EnergyTrace results remain required before a production-verified claim.
