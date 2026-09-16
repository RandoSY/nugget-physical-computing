# From 555 Timer to Reaction-Time Instrument
## Reformatted Teacher Tutorial Edition - Part 1 of 2

*Recovered searchable edition from the formatted teacher-tutorial PDF preserved in the Library.*

A structured pathway from clock, logic, and latch to a data-producing reaction-time instrument.

**Core teacher message:** A simple game becomes real science when the controller captures the moment, measures the delay, and reports the result.

The central concept graphic frames the machine this way: the 555 creates a timing window, the 74LS00 logic validates the response, and the latch remembers the result.

- 555 timer: visible timing.
- 74LS00: visible logic.
- Nano: simulation and instrument.
- PIC16F18424: REDBOARD target.

Each is a stage in the same learning trajectory, not a separate topic.

## 1. Executive Summary

The tutorial is intended as a professional guide for new teachers. A small electronics idea becomes a complete learning trajectory. The 555 timer supplies timing, the 74LS00 supplies logic, the SR latch supplies one-bit memory, and the microcontroller turns that logic into a measurable human reaction-time instrument.

Use the tutorial as a sequence, not as a pile of parts: clock, gate, latch, game, simulation, microcontroller, serial report, dashboard, and analysis.

Teacher value: time becomes signal, logic becomes rule, memory becomes state, and state becomes data.

Student value: students see why code exists. It compresses a visible machine into a programmable one without erasing the underlying idea.

Science value: reaction time becomes a repeated-trial distribution with median, variability, false starts, and fatigue or practice effects.

Safety value: the activity supports observation and comparison, not diagnosis. The score is a pattern under a protocol, not the person.

### Clean sequence

| Stage | Teacher focus | Learner insight |
|---|---|---|
| 555 timer | Clock and opportunity | Time can be made visible. |
| 74LS00 NAND | Logic and gating | A rule decides whether a signal counts. |
| SR latch | One-bit memory | A passing event can become stored state. |
| Wokwi/Nano | Simulation and debugging | The same machine can be tested before hardware. |
| Reaction timer | Measurement protocol | A human response can be captured as data. |
| Dashboard/Python | Distribution analysis | Repeated trials reveal pattern, not just one score. |

## 2. The Conceptual Spine

The most important teaching move is continuity. The 555, 7400, Nano, PIC, Wokwi, and dashboard are translations of the same small machine.

**Teacher framing:** Clock creates the opportunity. Logic decides whether the response counts. Memory keeps the result alive. Reporting turns the result into data.

| Concept | Plain-English meaning | Where students see it |
|---|---|---|
| Clock | Machine heartbeat or timing opportunity | 555 astable, software timer, random wait |
| Logic | Decision rule | NAND gate, IF statement, state-machine condition |
| Latch Q | One stored bit | SR latch, green success LED, reaction captured |
| Reset | Return to known state | Reset button, MCLR, new round |
| Stimulus | Signal shown to the person | GO LED |
| Response | Human action | User-switch press |
| Report | Measured result | `REACTION_MS,xxx` over serial |

Start with the visible machine. Only after students can explain clock, permission, latch, and reset should the teacher move to code. Code is then understood as a compressed version of the same idea, not a magic replacement for the circuit.

## 3. The Original Circuit as the Anchor Graphic

Use the 555 + 7400 circuit figure once, deliberately, as the anchor graphic. Ask: Where is the clock? Where is the permission rule? Where is the memory? What resets the system?

- **Block 1 - 555 clock:** A timing source creates repeated opportunities. The target light is driven by a clock.
- **Block 2 - buttons:** Human action becomes electrical input. The person is now part of the machine.
- **Block 3 - NAND latch:** Logic decides and memory holds. This bridges event to state.
- **Block 4 - output:** The success LED is stored state made visible. The machine remembers the successful moment.

## 4. Wokwi Simulation - Use the Working Screenshot Once

The tutorial retires artificial Wokwi illustrations in favor of the actual working screenshot as proof that simulation, serial output, buttons, LEDs, and buzzer came together in a real browser-based test.

Teaching purpose:

- prove the state machine can run before students build physical hardware;
- show the serial terminal as a scientific reporting channel, not only debugging;
- provide a concrete picture of successful behavior: latched win, buzzer feedback, and reaction-time report;
- remind teachers that Wokwi is best for logic and protocol validation, while real hardware should be used for serious reaction timing.

## 5. Arduino Nano Code Concepts

The Arduino version is the clean bridge between Wokwi and real hardware. It makes the primitive logic programmable and gives the teacher an approachable state-machine implementation.

```cpp
const int TARGET_LED = 8;   // red GO/target LED
const int SUCCESS_LED = 9;  // green latched-success LED
const int BUZZER_PIN = 10;  // buzzer or speaker
const int PLAY_BTN = 2;     // PLAY button to GND
const int RESET_BTN = 3;    // RESET/START button to GND

pinMode(PLAY_BTN, INPUT_PULLUP);
pinMode(RESET_BTN, INPUT_PULLUP);

// INPUT_PULLUP convention:
// not pressed = HIGH
// pressed = LOW
bool play = (digitalRead(PLAY_BTN) == LOW);
bool reset = (digitalRead(RESET_BTN) == LOW);
```

Software latch behavior:

```cpp
if (reset) {
  latchQ = false;
} else if (play && clockState) {
  latchQ = true;
}

digitalWrite(SUCCESS_LED, latchQ ? HIGH : LOW);
```

The key bridge is explicit: physical SR-latch Q becomes the software variable `latchQ`.

## 6. Reaction Game Upgrade

The pedagogic upgrade is the shift from "press while the light is blinking" to a true reaction task: get ready, wait through an unpredictable delay, respond only when GO appears, and report the measured response time.

- **Ready flashes:** three short LED flashes prepare the participant without inviting a response.
- **Random wait:** a dark delay prevents anticipation.
- **GO signal:** the LED turns solid ON and the microcontroller stores `goTime` locally.
- **Response capture:** button press gives `pressTime`; reaction time is `pressTime - goTime`.
- **False start:** a press before GO is reported separately.
- **Serial report:** result is sent as `REACTION_MS,xxx` after local measurement is complete.

```cpp
void triggerGo() {
  goTimeMs = millis();
  digitalWrite(LED_PIN, HIGH);
  Serial.println("GO,0");
}

void winRound() {
  unsigned long reactionMs = millis() - goTimeMs;
  Serial.print("REACTION_MS,");
  Serial.println(reactionMs);
}
```

Measurement integrity: serial delay does not change measured reaction time if the controller computes the time locally first.

## 7. Minimal One-LED / One-Switch Version

The minimal Nano version uses the built-in D13 LED and one switch on D2. The LED carries a complete state language:

| LED behavior | Meaning |
|---|---|
| 3 short flashes | Get ready; the trial is starting. |
| LED off | Random wait; do not press. |
| Solid on | GO; press the button now. |
| Fast flashes | Success; reaction was captured. |
| Long flashes | False start; reset or begin a new trial. |

```cpp
const int LED_PIN = 13;     // built-in Arduino Nano LED
const int BUTTON_PIN = 2;   // user switch to GND

bool buttonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}
```

Teacher point: minimal hardware is not a compromise. It forces the state machine to be clear enough to communicate through one light and one button.

---

**Provenance:** searchable repository-native edition recovered from `From_555_to_Reaction_Time_Teacher_Tutorial_Reformatted_v4.pdf`. The original formatted PDF, including figures and screenshots, remains preserved in the Library. Part 2 continues with Great Cow BASIC, REDBOARD/PIC, serial/dashboard analysis, teacher protocol, and classroom packaging.
