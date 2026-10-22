# Integrated Inquiry Capstone — The Transparent Scientific Instrument

**Challenge:** Build a command-controlled microinstrument that measures at least two physical quantities, reports structured data over the LaunchPad USB UART, and demonstrates one period-based task. At least one sensor must use I2C or SPI (the recommended VL6180X reference uses hardware I2C). A load-cell scale or ultrasonic/temperature experiment supplies the second measurement.

## Observable output

A serial transcript such as:

```text
ID EDU430-LAB MSP430G2553
DATA TIME_MS=1000 RANGE_MM=42 TEMP_C_X10=238
DATA TIME_MS=2000 RANGE_MM=43 TEMP_C_X10=238
ERROR I2C_NACK DEVICE=0x29
```

Use simple line-oriented values suitable for 512 bytes of MSP430 RAM. Data line above is a **format illustration**, not measured data.

## Required evidence

1. A photograph/sketch of the circuit and documented voltage compatibility.
2. A labeled macro-expansion diagram (application → Edu430 → TI register → electrical action).
3. A machine-level debugger trace of one assembly routine, showing PC, SP, flags and one general register.
4. A PDP-11 comparison: one precise similarity and one precise non-equivalence, with a primary manual citation.
5. A calibration table with 5 repeated readings per reference point, correct units and uncertainty.
6. A command/response capture and one injected fault (disconnect one bus device / unexpected NACK) handled without hanging.
7. A test statement distinguishing host syntax, MSP430 build, physical board demonstration and real sensor calibration.

## Minimal vs stretch

Minimal: one optical ToF plus one analog temperature channel; 1-Hz USB serial, interactive `READ` and `STATUS` commands, simple scheduler, clear fault response. Stretch: HX711 mass plus ultrasonic range, separate calibration constants, EnergyTrace power estimates, repeatable machine instruction timing.

## Suggested grading (100 points)

Functional demonstration 20; data quality/calibration 20; transparent macro/register tracing 15; assembly/PDP-11 reasoning 20; safety and error recovery 15; reproducibility and explanation 10.
