# Teacher extension: Edu430 FwLib

The first eleven lessons and four sensor appendices are unchanged.
This optional second-stage curriculum unifies earlier macro efforts from the
MSP430 Macro Evolution Manual, Nugget FwLib, and Piconomix PX-FwLib.

## 4-week investigation arc

1. **Read voltage as a bit**: use examples 01 and 08 to compare plain
   registers, generic bit macros, and semantic names.  Have students
   predict P1OUT/P1DIR changes, then step in CCS.
2. **Measure and report**: combine UART and ADC examples 02–03. Calibrate
   against a voltmeter; distinguish ADC counts, resolution, accuracy.
3. **Learn shared buses**: example 04 SPI memory, example 05 software-I²C,
   example 06 hardware I²C.  Have learners predict when P1.6 becomes LED,
   clock, or data; explain why USCI_B0 cannot run both SPI and I²C together.
4. **Schedule behavior without an RTOS**: example 07 and Lesson 11.
   Compare jitter, missed deadlines, EnergyTrace (G2ET only), and CPU sleep.

## Assessment and lab safety

Students should describe BOTH a macro and the exact register operation it
expands into; produce a wiring table with voltage levels; show a debugging
trace; and explain at least one measured uncertainty or failure mode.
Use only 3.3V-safe sensors and external pullups to 3.3V for I²C.  Test a
5V ultrasonic sensor only with level shifting.  Never erase InfoA.

## Evaluation rubric (four dimensions, 0–3)

- Behavior: students show a working physical signal/instrument.
- Mechanism: students explain pin mux, registers, clocks, masks.
- Evidence: students log measurements, timing and errors.
- Transfer: students adapt one macro across two examples or explain an MCU port.

## Verified and not verified

The new source passed host GCC syntax and deterministic logic tests using a
mock header. CCS imports and real connected devices were not tested here.
Before distributing as guaranteed classroom-ready, use CCS and both G2 and
G2ET to compile/flash each example and test buses and timeouts.
