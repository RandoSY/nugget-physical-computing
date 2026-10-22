# Edu430 VL6180X / hardware I2C exemplar — v0.3 add-on

- An **additive** example for the Edu430 v0.2 family. Original 11 lessons and 8 previous library examples are unchanged.
- Real USCI_B0 I²C bus: 7-bit addresses, write+repeated-START+read, and **16-bit sensor register addresses**.
- Device-neutral transport and VL6180X-specific driver separated using read/write function callbacks.
- 43 factory tuning register/value pairs (ST DT0037); model ID validation; single-shot range status checks.
- A self-contained CCS ProjectSpec, error codes, test harness and teacher guide.
- Host syntax and behavioral tests pass. The hardware I²C transaction engine has **not** been verified on an actual board and sensor.
- SPI and hardware I²C share USCI_B0; P1.6 also drives LaunchPad LED2.
- Original v0.2 driver remains; new code does not overwrite it.

License: GPL-3.0 in context of RandoSY MSP430 Learning Labs; ST/TI references are cited, not copied verbatim.
