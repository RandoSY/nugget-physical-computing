# Original MSP-EXP430G2 and newer MSP-EXP430G2ET

The **MSP430G2553** silicon GPIO/peripherals are the same target in both LaunchPads. Different USB interfaces and jumper paths are the main difference. Pin colors are deliberately named `LED_A` (P1.0) and `LED_B` (P1.6); confirm the actual color on your board. For standard boards, the G2 has P1.0 red / P1.6 green, and the G2ET schematic uses P1.0 green / P1.6 red. The software NEVER assumes a color.

- Onboard button: P1.3, pull-up enabled, pressed reads zero.
- USCI_A0 UART: P1.1 RXD and P1.2 TXD; check LaunchPad TXD/RXD backchannel jumpers.
- LEDs: P1.0 and P1.6; lesson 06 timer PWM uses P1.6 TA0.1.
- ADC A4: P1.4. External input must stay between GND and MSP430 VCC, NOT 5V.
- P2.0/P2.1: free GPIO for HX711 or software I2C, but not concurrently.
- P2.3/P2.4: sonar trig/echo; level shift the 5 V echo signal.
- Avoid using 5V signals on MSP430 pins. Tie sensor and LaunchPad grounds together.

Cross-board success criteria: same hex file/device target, observable GPIO outputs, responsive button, ADC reading, UART terminal with the correct connection and jumpers. Confirm voltage and pin mapping before wiring accessories.
