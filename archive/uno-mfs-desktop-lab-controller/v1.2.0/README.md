# UNO / MFS Universal Desktop Lab Controller — v1.2.0 Recovery

**Platform:** Arduino UNO R3 / ATmega328P + classic Multi-Function Shield  
**Historical release:** firmware v1.2.0, September 2026  
**Recovery disposition:** public-safe source recovered; validation status preserved without upgrade.

This is a four-instrument desktop laboratory controller that keeps one inexpensive hardware/front-panel platform constant while the measurement problem changes.

## Instruments and universal wiring

- **TEMP:** DS18B20 on D6 with 4.7 kΩ pull-up
- **SCALE:** HX711 bridge ADC, DOUT D9 / SCK D2
- **DIST:** HC-SR04 on a protected shared D5 node; TRIG direct, ECHO through 2.2 kΩ
- **LIGHT:** BH1750 on I²C A4/A5

The classic MFS buttons remain the operator interface: SW1 advances instrument mode; SW2 performs the mode-specific primary action; SW3 toggles HOLD/LIVE, with a long SW3 press in SCALE mode invoking the 500 g calibration workflow.

## Architecture

The firmware uses a cooperative scheduler rather than an RTOS. Normal sensor services are bounded/nonblocking, while explicit HX711 tare/calibration transactions remain finite and continue display/watchdog service. The implementation avoids Arduino `String` and dynamic allocation on the UNO's 2 KB SRAM.

A valid DS18B20 ambient-temperature reading can improve the sound-speed model used by ultrasonic distance measurement, so the four sensors are not treated as completely isolated channels.

## Host and protocol model

USB serial and an HM-10 connected to the MFS UART header carry the same line-oriented ASCII protocol over the UNO hardware UART at 9600 baud.

The controller powers up in **LOCAL** ownership so it remains a usable stand-alone instrument without a host. Observation commands such as READ/STATUS/STREAM remain available locally; state-changing commands require **CONTROL REMOTE**. Reset or `CONTROL LOCAL` returns ownership to the physical operator.

Protocol-v2 records include node identity, sequence, batch time, sample time, instrument, value and unit. The firmware also exposes identity, capabilities, session, health/self-test and watchdog/reset provenance.

## Recovered source

The exact recovered source package is under `source-archive/`, reconstructed from deterministic base64 chunks with SHA-256 verification. It contains:

- the 1667-line Arduino v1.2.0 application;
- the loopback-only Python Arduino-CLI bridge;
- the surviving v1.2 architecture note.

## Validation boundary

Surviving documentation records host g++/clang++ compilation, AVR-target relocatable-object compilation and firmware-contract checks as PASS. It records final Arduino AVR core link/HEX as unavailable in that environment and physical UNO/sensor bench acceptance as still required.

That is the preserved release status.
