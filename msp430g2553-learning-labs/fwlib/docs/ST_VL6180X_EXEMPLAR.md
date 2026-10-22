# ST VL6180X: From an I²C Wire to a Reusable Optical Sensor Driver

## Why this becomes our I²C reference lesson

I²C is a gateway to temperature, barometric, light, proximity, IMU,
magnetic-field, and environmental sensors. A credible general-purpose library
must handle repeated START and both 8-bit and 16-bit register addresses.

The **VL6180X**, rather than a generic "ST ToF sensor", is deliberately
selected because it uses **16-bit register addresses**, has a verifiable **model
ID 0xB4**, requires a manufacturer startup tuning sequence, and reports distance
in **millimeters**. A VL53L0X / VL53L1X uses a different driver.

## Physical diagram (I2C pins and UART are independent)

```text
             original G2 / newer G2ET LaunchPad
                     MSP430G2553
                 +-----------------+        VL6180X BREAKOUT
 P1.6 / UCB0SCL |-----------------+-------- SCL
 P1.7 / UCB0SDA |-----------------+-------- SDA
             GND|-----------------+-------- GND
             3V3|-----------------+-------- VIN *only if rated*
                 |                 |
 P1.1 / UCA0RXD |<----- on-board USB UART backchannel
 P1.2 / UCA0TXD |-----> 9600-baud Windows COM terminal
                 +-----------------+

  SCL ---- 4.7 kOhm ---- +3.3 V  (unless supplied by breakout)
  SDA ---- 4.7 kOhm ---- +3.3 V  (unless supplied by breakout)
```

**Remove/disconnect the P1.6 LED2 jumper** when hardware I²C is being used.
**Never apply 5 V** to a G2553 GPIO pin or I²C wire. Check the specific breakout:
the **bare VL6180X** has its own lower-voltage supply requirements; a board
must explicitly support 3.3 V I/O and the chosen VIN voltage. I²C pullups
must go to 3.3 V, not 5 V. Ground must be shared.

### One measurement as a ladder

**Application:**

```c
result = edu_vl6180x_range_mm(&bus, &millimeters, 200u);
```

**Device layer:**

```c
bus->read(bus->context, 0x0062u, &millimeters);
```

**Bus layer (16-bit register):**

```c
edu_i2c_hw_read_reg16(0x29u, 0x0062u, &millimeters, 60000u);
```

**On the wire:**

```text
START  [0x29 + WRITE] ACK [0x00] ACK [0x62] ACK
REPEATED START [0x29 + READ] ACK [range_byte] NACK STOP
```

**Registers:** `UCB0SA`, `UCB0CTL1` (`UCTR`, `UCTXSTT`, `UCTXSTP`),
`UCB0TXBUF`, `UCB0RXBUF`, `IFG2`, and `UCB0STAT` (`UCNACKIFG`).

**Physical interpretation:** A START is SDA falling while SCL is high;
SDA/SCL require pullups. Repeated START changes bus direction while retaining
ownership. The final NACK closes a one-byte read; STOP releases the bus.

### Main steps of the sensor algorithm

1. Start the 1 MHz clock, UART, and USCI_B0 I²C (50 kHz).
2. Read model ID register `0x0000`; verify value `0xB4`.
3. Read `SYSTEM__FRESH_OUT_OF_RESET` at `0x0016`.
4. When fresh, write ST's specified tuning-register table (43 pairs), then
   record initialization by writing `0x00` to `0x0016`.
5. Start single-shot ranging with `0x0018 = 0x01`.
6. Poll bits 2:0 of `0x004F` for `0x04`, with a bounded wait.
7. Check error bits in `0x004D`, read millimeters from `0x0062`.
8. Clear the finished interrupt using `0x0015 = 0x07`.
9. Print distance or a meaningful failure marker through USB UART.

### Three reusable pieces

| Component | Source | What students learn |
|---|---|---|
| USCI_B0 bus | `include/edu430_i2c_reg16.h` | Generic address/write/read with repeated START |
| ST device | `include/edu430_vl6180x.h` | Sensor identity, calibration, range status and units |
| Classroom application | `examples/09-vl6180x-hardware-i2c/main.c` | Code Composer, calibration table, UART CSV |

The device driver has *no* `P1OUT` or `UCB0` references. A bus callback separates
I²C *transport* from sensor *semantics*, following the design of PX-FwLib.

### STEM investigations

- **Recognition:** Read `0x0000` from the optical sensor. What happens when
  disconnected, or when P1.6 LED jumper is still installed?
- **Ranging:** Record 10 samples each at 20, 40, 60, 80, and 100 mm against
  a ruler. Graph measured versus nominal range; discuss systematic errors.
- **Surfaces:** Compare a white card, black card, and reflective material at
  matched distances. What is the effect of surface reflectivity?
- **Environment:** Investigate ambient illumination and viewing angle.
- **Time:** Measure response latency and compare with the HC-SR04 ultrasonic
  appendix. Do not equate sound flight time with optical direct ToF timings.
- **EnergyTrace:** Compare active polling against timed tasks and sleep.

### What is validated and what isn't

- Source syntax can be checked on a host C compiler with a mocked MSP430
  device header; this does **not** prove I²C bus waveforms are correct.
- Mock sensor tests verify model detection, tuning writes, bounded polling,
  successful millimeter reports, and failure paths without physical hardware.
- CCS ProjectSpec is a prepared import candidate, not verified in CCS 21.
- The experimental hardware bus uses one-byte receiver operation as its
  reference. The G2553 USCI30 erratum requires special care on multi-byte RX;
  do not claim arbitrary multi-byte transactions are validated.
- Host polling loops have iteration-count timeouts, not wall-clock timeouts.
- No multi-master arbitration, DMA, ISR-based I²C, or unwedging a physically
  stuck SCL line is supplied. The driver is a **teaching reference**, not yet
  a production-certified communication stack.

### Hands-on validation checklist

1. Confirm 3.3 V at target and on both pullup resistors; check wiring.
2. Confirm P1.6 LED jumper disconnected and correct 0x29 (7-bit) address.
3. Use CCS single-step at `edu_i2c_hw_read_reg16()`; monitor `UCTXSTT` and
   `UCTXSTP` without setting breakpoints in a way that corrupts timing.
4. Capture logic-analyzer transactions showing two register-address bytes,
   repeated START, then one data byte, NACK, STOP.
5. Read `0xB4` from model ID and check that all 43 settings are acknowledged.
6. Test valid range, sensor disconnected, NACK, and measurement timeout.
7. Verify no unexpected resets and that UART still prints at 9600 8N1.
8. Capture compiler version, board model, sensor breakout vendor, measured
   current draw, and test results in a lab record before designating verified.

### Authoritative technical references

- ST **VL6180X Datasheet**: https://www.st.com/resource/en/datasheet/vl6180x.pdf
- ST **DT0037** quick setup: https://www.st.com/resource/en/design_tip/dm00123019-vl6180x-range-and-ambient-light-sensor-quick-setup-guide-stmicroelectronics.pdf
- TI **SLAA382A** USCI I2C master: https://www.ti.com/lit/an/slaa382a/slaa382a.pdf
- TI **SLAU144** MSP430x2xx family user guide.
- TI **SLAZ440K** MSP430G2553 errata: https://www.ti.com/lit/er/slaz440k/slaz440k.pdf

This is an original educational implementation; no TI or ST code is copied
verbatim. It is offered under the surrounding curriculum's GPL-3.0 terms.
