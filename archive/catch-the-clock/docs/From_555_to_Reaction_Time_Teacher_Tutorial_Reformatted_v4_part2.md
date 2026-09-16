# From 555 Timer to Reaction-Time Instrument
## Reformatted Teacher Tutorial Edition - Part 2 of 2

*Recovered searchable edition from the formatted teacher-tutorial PDF preserved in the Library.*

## 8. Great Cow BASIC Translation

The Great Cow BASIC version carries the same logic into the AVR/Nano and later PIC/REDBOARD world. It makes the tutorial portable across the Nugget pathway.

### GCBASIC serial reliability and pin map

```text
#chip mega328p, 16

#define USART_BAUD_RATE 9600
#define USART_TX_BLOCKING

#define TARGET_LED PORTB.0   ' Nano D8
#define SUCCESS_LED PORTB.1  ' Nano D9
#define BUZZER_PIN PORTB.2   ' Nano D10

#define PLAY_BTN PORTD.2     ' Nano D2
#define RESET_BTN PORTD.3    ' Nano D3
```

### GCBASIC latch logic

```text
If ResetState = 1 Then
   LatchQ = 0
Else
   If PlayState = 1 Then
      If ClockState = 1 Then
         LatchQ = 1
      End If
   End If
End If

SUCCESS_LED = LatchQ
```

Implementation note: the practical discovery was that 9600 baud plus `USART_TX_BLOCKING` produced clean Wokwi serial output. The tutorial treats that as a useful teacher-debugging lesson.

## 9. REDBOARD / PIC16F18424 Version

The REDBOARD version is designed to use its built-in resources cleanly: reset, one user switch, red/yellow/green LEDs, buzzer, and RC0 serial output. RC0 is the measurement/report channel and should not be shared with a human-interface load.

| Resource | Role |
|---|---|
| RESET/MCLR | Starts over and restores a known state. |
| User switch | Response input; early press is false start, post-GO press is valid. |
| Red LED | Stop, error, or false-start cue. |
| Yellow LED | Armed/waiting cue. |
| Green LED | GO signal and/or success latch depending on final design. |
| Buzzer | State feedback: ready, GO, win, false start. |
| RC0 serial TX | Reports `READY`, `GO`, `REACTION_MS`, `FALSE_START`. |
| GND | Common reference with Nano bridge or serial adapter. |

### REDBOARD serial-separation rule

```text
' Architectural rule:
' RC0 is serial TX only. Do not share it with LED, buzzer,
' switch, display segment, or multiplex line.

#define SERIAL_TX    PORTC.0  ' RC0: report channel only
#define RED_LED      PORTC.1  ' example human-interface pin
#define YELLOW_LED   PORTC.2  ' example human-interface pin
#define GREEN_LED    PORTC.3  ' example human-interface pin
#define BUZZER_PIN   PORTA.4  ' example only
#define USER_SWITCH  PORTA.2  ' example only
```

This separates the human interface from the data channel and prevents buzzer or LED activity from corrupting serial reporting.

## 10. Serial Protocol and Web Dashboard

The dashboard and CSV export turn the instrument into a data-producing lab. The serial protocol is intentionally short, consistent, and parseable by a person, browser dashboard, or Python.

| Event line | Meaning |
|---|---|
| `READY,0` | Device has booted and is ready. |
| `ROUND_START,0` | A new trial has begun. |
| `READY_FLASHES,3` | Participant receives get-ready cue. |
| `WAIT_START,0` | Random dark wait has begun. |
| `GO,0` | GO signal has appeared. |
| `REACTION_MS,312` | Valid response captured; reaction time is 312 ms. |
| `FALSE_START,0` | Button was pressed before GO. |
| `NEXT,0` | Device is ready for another trial. |

Dashboard role:

- connect by Web Serial to Nano, REDBOARD bridge, or serial adapter;
- collect trials and false starts as a session dataset;
- compute summaries such as best, median, mean, standard deviation, IQR, coefficient of variation, and trend;
- export CSV for upstream Python analysis.

Example stream:

```text
READY,0
ROUND_START,0
READY_FLASHES,3
WAIT_START,0
GO,0
REACTION_MS,312
NEXT,0
```

## 11. Reaction-Time Analysis for Teachers

The teacher should move students away from a single score and toward a distribution. A serious classroom protocol uses repeated trials and separates valid responses from false starts.

| Measure | What it tells the teacher |
|---|---|
| Best / minimum | Fastest observed response under the protocol. |
| Median | Typical response; robust against slow lapses. |
| Mean | Average response; useful but sensitive to outliers. |
| Standard deviation | Trial-to-trial consistency. |
| IQR | Robust middle-50-percent spread. |
| False starts | Anticipation, inhibition, or misunderstanding. |
| Trial trend | Practice effect, fatigue, or attention drift. |

Recommended protocol:

- run 3-5 practice trials;
- collect 20-40 valid trials;
- record false starts separately;
- use median as the main summary value;
- discuss variability and drift as important outcomes, not merely errors;
- compare a person to their own baseline before comparing people to each other.

**Professional caution:** this measures simple visual reaction time under a protocol. It is not a medical diagnostic device.

## 12. Teacher Tutorial Structure

For teacher training, the tutorial should be run as a sequence of teaching moves rather than an engineering demo. The teacher learns how to narrate one concept across hardware, code, and data.

| Move | Teacher action |
|---|---|
| Show | Project the original 555 + 7400 diagram and identify blocks. |
| Explain | Define clock, logic, latch, reset, and output in plain English. |
| Simulate | Use the actual Wokwi project to demonstrate behavior and serial report. |
| Build | Move to Nano or REDBOARD hardware. |
| Measure | Run a simple reaction-time protocol. |
| Analyze | Use median, variability, false starts, and trend. |
| Reflect | Ask what the microcontroller made possible that the simple circuit could not do easily. |

Suggested teacher talk track:

> We built a machine that waits, signals, listens, remembers, and reports. Then we used it to study ourselves.

> The microcontroller is not replacing the 555 and NAND gate. It is carrying their ideas forward into a programmable instrument.

## 13. Classroom Resource Package

The final curriculum package is organized as a teacher kit to reduce friction for new teachers while preserving the intellectual depth of the path.

| Resource | Role |
|---|---|
| Concept graphic | 555 + 74LS00 Catch-the-Clock figure. |
| Simulation project | Working Wokwi Nano setup with screenshot and serial output. |
| Arduino sketch | Nano implementation with LEDs, switch, buzzer, and serial report. |
| Minimal sketch | D13 LED + D2 switch reaction timer. |
| GCBASIC source | AVR/Nano and REDBOARD/PIC pathway. |
| Dashboard package | Web Serial collection, CSV export, Python analysis. |
| Teacher guide | This structured learning-trajectory document. |
| Student protocol sheet | Trial-count, false-start, and reflection worksheet. |

### Final instructional claim

The path is powerful because nothing is wasted. The 555 timer remains the clock idea. The 7400 remains the logic idea. The SR latch remains the memory idea. The Nano, PIC, dashboard, and Python analysis simply make those ideas repeatable, measurable, and useful.

---

**Provenance:** repository-native searchable edition recovered from `From_555_to_Reaction_Time_Teacher_Tutorial_Reformatted_v4.pdf`. The original formatted PDF, with diagrams and screenshots, remains preserved in the Library. This text edition preserves instructional content rather than exact page layout.
