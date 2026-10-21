# Advanced optional optical ToF: ST VL6180X

This appendix is experimental and uses software I2C instead of hardware USCI I2C. It is **for VL6180X only**, not VL53L0X or VL53L1X. Uses ST-recommended VL6180X startup register sequence from [DT0037](https://www.st.com/resource/en/design_tip/dt0037-vl6180x-range-and-ambient-light-sensor-quick-setup-guide-stmicroelectronics.pdf). Expected distance is approximately 0-100 mm in suitable optical conditions, not room-scale ultrasonic ranges.

**Electrical:** Use a breakout explicitly rated for **3.3V logic** (on-chip supply may be lower); connect SDA to P2.0, SCL to P2.1, GND common, and add/verify pullups from both lines to 3.3 V (typically 4.7 kOhm). Never pull these MSP430 pins to 5 V. Verify breakout power specs. Serial output is 9600 8N1 on the LaunchPad backchannel.

**Driver lesson:** `low()` changes P2DIR to drive low, `release()` turns the pin into an input. This is open-drain behavior. `write_reg()` uses 16-bit register addresses and `sensor_init()` applies the manufacturer's boot settings. Compare this to UART hardware in lesson 08.

**Status:** Source-level host syntax checks only; no physical sensor or CCS target compilation performed. For a beginner's first distance exercise, use Appendix C HC-SR04 instead.
