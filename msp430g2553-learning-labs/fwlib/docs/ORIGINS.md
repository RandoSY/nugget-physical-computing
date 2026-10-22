# Origins: consolidate rather than reinvent

This mini library brings together three previously explored teaching directions:

- A 2026 **MSP430G2553 Macro Evolution Manual**, historically including an `msp430_easy.h`
  API across GPIO, timers, ADC10, Comparator_A+, UART, SPI, I²C, watchdog and flash.
  It introduced the key ladder: **board intent → chip helper → register expression → voltage**.
- [Piconomix PX-FwLib](https://github.com/piconomix/px-fwlib): small, reusable
  architecture/board/common/comms-oriented components and a readably portable bare-metal API.
- [Nugget FwLib](https://github.com/RandoSY/nugget-fwlib): behavior-centric modules,
  from switch/LED and timed events to command/response instruments.

This implementation is written specifically for the MSP430G2553 rather than mechanically
copying code designed for another microcontroller. GPIO is preprocessor-friendly; timeouts
and protocol transactions are small functions, because obscure macros would be harder to teach.

The Piconomix upstream repository is MIT licensed; these new files are original teaching
adaptations inside a GPL-3.0 course. Preserve both attributions when you redistribute.

## Deliberate exclusions

- No malloc, heap allocation, RTOS, or preemptive scheduler.
- No macro that silently erases MSP430 information flash, including InfoA calibration.
- No promise that a single 20-pin package exposes every P3 signal.
- No false claim that USCI_B0 provides separate, simultaneously usable I²C and SPI engines.
- No transparent auto-generated baud rate unrelated to the chosen SMCLK.
- No unqualified energy/current measurement claims without EnergyTrace data.
