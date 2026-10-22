# Teacher's guide: configuring CCS and the two LaunchPads

## Quick setup (Windows)

1. Connect the appropriate LaunchPad debugger to USB.
2. Open Code Composer Studio with the MSP430 device support installed.
3. Import an example's `.projectspec` if supported by your installed CCS version;
   otherwise create an empty MSP430 C project targeting **MSP430G2553**.
4. Add the selected `main.c` and the `.h` files **from the same example directory**.
5. Confirm target board/debug probe (new G2ET uses eZ-FET). Build, flash, then run.
6. For serial lessons, select the backchannel COM port and use **9600 8N1**.
7. Do not compile examples alongside the existing lessons: each has its own `main()`.

The ProjectSpecs are provided as import candidates, not claimed to have been tested
with every CCS release. They follow the same minimal format as Lesson 11.

## Hardware pin map and conflicts

| Feature | MCU pins | Critical warning |
|---|---|---|
| LED A / user button | P1.0 / P1.3 | Button is active-low with pull-up |
| LED B | P1.6 | Shares pad with SPI MISO and hardware-I²C SCL |
| UART backchannel | P1.1 RX / P1.2 TX | Set board UART RX/TX jumpers correctly |
| ADC10 A4 | P1.4 | Voltage must be between ground and VCC |
| SPI USCI_B0 | P1.5 CLK / P1.6 MISO / P1.7 MOSI | P1.6 LED conflicts with SPI |
| Hardware I²C | P1.6 SCL / P1.7 SDA | Cannot operate concurrently with USCI_B0 SPI |
| Software I²C | P2.0 SCL / P2.1 SDA | Two external ~4.7k pullups to 3.3V |

**Never connect a 5V echo or other 5V logic signal directly to an MSP430 input.**
Do not use 5V pullups on the I²C bus. Share GND with external modules.

The original G2 and G2ET have the same G2553 peripheral registers and most of the
same onboard GPIO mapping, but differ in debugger, button labels, LEDs and connectors.
The original's debugger may require legacy toolchain arrangements; the newer G2ET
supports EnergyTrace using its onboard eZ-FET.

## Lesson sequence: use a measurement question each time

1. What actual voltage change corresponds to `EDU_PIN_ON()`?
2. What makes a UART character arrive at a chosen rate?
3. How do ADC counts respond to known voltages? Estimate slope and offset.
4. Which bytes does SPI send? Why must a chip-select line exist?
5. Can an I²C scanner detect an unplugged module? What is a valid ACK?
6. Why do SPI and I²C fight for P1.6/P1.7? What is pin multiplexing?
7. How do timed tasks interfere? How does unsigned wraparound work?
8. Can a learner expand the entire program into direct C register operations?

## Validation and debugging

- First check 3.3V power, common GND, jumper orientation, and peripheral pin assignment.
- Then build for *actual MSP430G2553*, not a desktop compiler.
- On SPI, monitor P1.5 CLK, P1.7 SIMO, and P2.0 chip select.
- On I²C, monitor START/STOP, ACK/NACK, SCL stretching and pull-up edges.
- Confirm real I²C read transactions against the device's register protocol.
- Confirm that `edu_i2c_read_reg8` matches the register-width of your actual sensor.
- Try timeouts with the device disconnected, and record error handling.
- Inspect current MSP430G2553 errata (especially USCI30/34/35) before production use.

## Validation categories

- **Host syntax verified:** GCC with an emulated memory-register header, no target binary.
- **CCS compiler verified:** must be performed by teacher/maintainer on selected toolchain.
- **Bench verified:** must be performed on G2 and G2ET with real modules and serial terminal.
- **Qualified library:** would additionally require prolonged error/recovery/load tests.

The source provided here belongs in the first category. Do not imply bench verification.
