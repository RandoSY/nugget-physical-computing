# Source references / further reading

- Texas Instruments MSP430G2553 datasheet (SLAS735J):
  https://www.ti.com/lit/ds/symlink/msp430g2553.pdf
- Texas Instruments MSP430x2xx Family User's Guide (SLAU144):
  https://www.ti.com/lit/ug/slau144j/slau144j.pdf
- Texas Instruments MSP430G2553 errata (SLAZ440):
  https://www.ti.com/lit/er/slaz440k/slaz440k.pdf
- Texas Instruments G2ET LaunchPad User's Guide (SLAU772):
  https://www.ti.com/lit/ug/slau772/slau772.pdf
- Piconomix FW Library: https://github.com/piconomix/px-fwlib
- Original ADolbyB labs: https://github.com/ADolbyB/msp430g2553-labs
- This course's earlier `evolution/four-mode-led-patterns` examples.

The MSP430G2553 **USCI_B0** multiplexes SPI and I²C on the same peripheral.
The USCI I²C errata warrant conservative clocks and physical verification.
These links explain *why* the library chooses explicit pins and bounded waits.
