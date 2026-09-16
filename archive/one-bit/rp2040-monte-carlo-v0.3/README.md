# UMA RP2040-Zero One-Bit Monte Carlo Timing Engine — v0.3

**Recovered historical source — September 2026 estate rescue.**

Target: Waveshare RP2040-Zero or compatible clone using MicroPython, RP2040 PIO, USB serial, onboard WS2812/NeoPixel and a user button.

The exact recovered `main.py` is preserved losslessly as `main.py.gz`. Restore it with:

```sh
gzip -dc main.py.gz > main.py
```

Recovered source SHA-256:

```text
4a7f7ade5d2bfd9ca40f04d2e513e0f8c5e14985cf4ff14d2699795bccf4fd0a  main.py
82fce46c78b3efd431117a7394ddfcc378ee61efc22cc8f78f1f8a16e1acca4e  main.py.gz
```

## v0.3 corrections

1. Removed the blocking 20 ms sample LED flash from automatic sampling.
2. Replaced blocking `readline()` with a nonblocking serial character buffer.
3. Made PIO direct-sample and latch-transaction modes mutually exclusive.
4. Documented that Python overhead remains outside the requested random dwell.
5. Stops / tri-states PWM when switching to external-source modes.
6. Adds a `MAX_TRIALS` cap of 100,000 and warns when requests exceed it.

## Recovered pin assumptions

```text
NeoPixel / WS2812: GP16
User button:       GP14   # verify with BUTTON?
PWM output:        GP0
Sample input:      GP4
Latch reset:       GP2
Latch sample:      GP3
Latch Q input:     GP4
Debug pulse:       GP5
```

## First loopback test

Jumper `GP0 -> GP4`, share ground, then over USB serial:

```text
SET MODE PIO_DIRECT_INTERNAL_PWM
SET FREQ 1000
SET DUTY 37
SET TRIALS 1000
SET REPORT_EVERY 25
RESET
RUN
```

The estimated HIGH fraction should converge toward approximately `0.37`.

## Hardware boundary

RP2040 GPIO is 3.3 V only. A 5 V 555 or 7400-family output must not be connected directly to GP4; use 3.3 V logic or appropriate level shifting.

The source itself says its board-pin assumptions must be bench-verified on the exact clone. That limitation is preserved here rather than upgrading this historical artifact to a fully hardware-qualified release.
