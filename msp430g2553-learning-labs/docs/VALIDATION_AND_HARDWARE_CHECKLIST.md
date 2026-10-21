# Validation summary and target qualification checklist

**Build status (2026-10-21):** All ten lesson C sources, four sensor appendices and three evolution variants passed GCC C99 **host syntax-only** checks with a simulated header (`python3 tools/check_sources.py`: **17/17 PASS**). This is a parsing check, not a Texas Instruments compiler build, and does not prove correct peripheral function or flash/RAM fit.

**Not yet verified:** TI MSP430 target compiler, CCS 21 IDE project import, flash/debug on G2 and G2ET, UART bridge drivers/baud across both board variants, ADC correctness, Timer_A vectors and PWM pin mapping, HX711 sampling and sign extension, HC-SR04 timing/wiring, VL6180X startup I2C and optical ranging.

**Teacher's acceptance checklist** (do not mark PASS until physically demonstrated):

- [ ] Record installed CCS and MSP430 compiler versions and target MSP430G2553 configuration.
- [ ] Blink and read button on original MSP-EXP430G2; record debugger arrangement and board revision.
- [ ] Blink and read button on MSP-EXP430G2ET with eZ-FET debugger.
- [ ] Confirm P1.0 and P1.6 LED color/behavior on each board without hard-coded color assumptions.
- [ ] Check 1MHz DCO calibration values are non-erased and blink approximately timed.
- [ ] Check interrupt count and PWM waveform with suitable instrument.
- [ ] Open Windows COM port; verify both transmit and receive at 9600/8N1 and jumper positions.
- [ ] Measure VCC with a DMM and calibrate TMP36 measurement.
- [ ] Tare HX711, calibrate with reference masses, measure repeatability and offset drift.
- [ ] Install 5V-to-3.3V ECHO level shifting before connecting HC-SR04 to MSP430.
- [ ] Use a 3.3V-I/O-compatible VL6180X breakout; measure I2C lines and verify model ID.
- [ ] Check final linker flash/RAM usage on the MSP430G2553 (16KB flash, 512B RAM).

Report test results and environment before presenting this as a production-validated teaching release.
