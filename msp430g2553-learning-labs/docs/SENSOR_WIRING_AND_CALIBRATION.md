# Inquiry STEM appendices: wiring, calibration, and uncertainty

**Common engineering practice:** Draw a circuit first, power off while wiring, confirm ground, measure VCC with a DMM, identify units, make multiple measurements, record known standards, and compare residual errors. The numerical conversion printed by a sensor is not an accuracy guarantee.

## A. TMP36 analog temperature probe
- TMP36 breakout / bare package datasheet orientation must be verified from its manufacturer pinout; connect +VS to **3.3V**, GND to ground, VOUT to **P1.4 (A4)**.
- Voltage equation `T_C = (Vout_mV - 500) / 10`; source transmits `temp_C_x10` so 237 means 23.7 C.
- Assumes ADC reference = VCC = 3.300 V. Measure actual VCC and change `3300UL` constant accordingly. Sensor supply and ADC reference uncertainty contribute to overall error; ideally calibrate at two known temperatures.
- Do NOT connect a thermocouple directly or assume TMP36 formula applies to LM35/DS18B20.

## B. HX711 bridge amplifier + load cell
- Use a 3.3V-capable HX711 module; connect VCC to 3.3V, common GND, **DOUT to P2.0**, **PD_SCK to P2.1**. Wire E+/E- and A+/A- to the load cell **using the load cell's own wiring diagram**, since wire colors differ.
- The HX711's 24-bit signed ADC reading is converted using `grams=(raw-empty_offset)/counts_per_gram`. **The shipped values are PLACEHOLDERS, not calibrated readings.** Gather at least 20 unloaded readings; average. Place a certified or reasonably known mass, average again, compute `counts_per_gram=(loaded_mean-empty_mean)/mass_grams` (may be negative). Update the macros before claiming grams.
- Repeat measurements and look for offset drift, vibration sensitivity, hysteresis, eccentric loading, resolution and noise. A single-point slope is a first approximation; two or more known masses are better.
- High phase of PD_SCK should stay below the HX711 power-down threshold (~60 us). The short read loop disables interrupts to avoid stretching pulses. Do not mix with critical real-time activities without redesign.

## C. HC-SR04 ultrasonic ranging
- HC-SR04 normally requires **5 V**, TRIG **P2.3**, ECHO **P2.4** only through a resistor divider or 3.3 V level shifter. Suggested divider: **10 kOhm from 5V ECHO to P2.4 and 18 kOhm from P2.4 to GND**, producing about 3.21 V nominal. Tie grounds together. Check 5 V tolerance and hardware before connection.
- Program measures round-trip echo pulse in microseconds with Timer1_A; approximate distance in cm = microseconds / 58 near 20 C. Temperature affects sound speed and therefore distance.
- Plot measured distance against a ruler for 5, 10, 20, 40, 80 cm; estimate bias and scatter; compare soft and hard reflector surfaces.
- A timeout / missing echo is NOT a zero-distance measurement. Minimum ping spacing >=60 ms, implemented as 100 ms.

## D. ST VL6180X short-range optical time-of-flight
- This is **an STMicroelectronics sensor, not an STM32 microcontroller**. VL6180X is a short-range optical sensor; it does NOT share the initialization with VL53L0X.
- Get a breakout that explicitly supports **3.3 V MCU I/O**; bare sensor operating voltages differ. SDA **P2.0**, SCL **P2.1**, external or breakout pull-ups **to 3.3 V only**, GND common. Verify breakout voltage regulator and logic level shifting.
- The program implements an educational GPIO open-drain I2C controller and ST VL6180X startup sequence; it is an **advanced, hardware-unvalidated appendix**. Start with a short 20-100 mm measurement scale and examine target color/reflectance.
- Apply manufacturer-approved settings and check status/error codes. Optical material and ambient conditions matter. See ST DT0037 and AN4545.

## Classroom data workbook prompts
Record table columns `trial, reference, reading, residual, notes`. For each experiment ask: What physical quantity is sensed directly? Which assumptions turn voltage/counts/time into a useful unit? What are resolution and repeatability? What calibration or environmental factor dominates? What happens when a sensor times out or saturates?
