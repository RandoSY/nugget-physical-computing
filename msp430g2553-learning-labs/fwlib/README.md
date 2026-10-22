# Edu430 FwLib — From Bits to Behavior

**An optional, bare-metal MSP430G2553 library that keeps the electrical hardware visible.**

This library builds on our earlier *MSP430G2553 Macro Evolution Manual* and adopts the
layered organizing philosophy of [Piconomix PX-FwLib](https://github.com/piconomix/px-fwlib).
It extends, rather than replaces, the established eleven-lesson teaching curriculum.

## The original teaching question

A new student reads `P1OUT ^= BIT0;` and may not know what it does.
The same physical operation can be taught as a four-step sequence:

```c
// 1. Intent (application/board layer)
EDU_LED_A_TOGGLE();

// 2. General-purpose GPIO interface
EDU_PIN_TOGGLE(P1, BIT0);

// 3. General register helper
EDU_TOGGLE(P1OUT, BIT0);

// 4. Exact C register operation
P1OUT ^= BIT0;
```

**The objective is never to hide the fourth line.** Students should be able to peel away
one layer at a time, right down to a data-sheet register and a voltage change.

## What ships in v0.2 teaching preview

| Module | Student-facing API | Implementation |
|---|---|---|
| Bit helpers | `EDU_SET`, `EDU_TOGGLE`, `EDU_ALL_SET` | Preprocessor masks and register operations |
| GPIO | `EDU_GPIO_PULLUP`, `EDU_PIN_ON` | P1/P2/P3 register macros (P3 package-limited) |
| Port interrupts | `EDU_GPIO_IRQ_ENABLE`, `EDU_GPIO_IRQ_FALLING` | P1/P2 flags and edge selectors |
| System clocks | `edu_clock_1mhz()` | DCO factory calibration check |
| Watchdog and low power | `EDU_WATCHDOG_STOP`, `EDU_SLEEP_LPM0` | WDT and CPU status-register macros |
| PWM | `edu_pwm_ta0_p16_begin`, `edu_pwm_ta0_p16_duty` | TA0.1, 1–999 permille (P1.6) |
| Comparator_A+ | `EDU_COMP_OUTPUT_HIGH`, `EDU_COMP_ON` | Comparator register macros |
| Button debounce | `edu_button_pressed` | Sampled-event state machine |
| Timer_A | `EDU_TIMER_UP`, `EDU_TIMER_1MS_TA0` | CCR0, CCTL0, CTL, interrupts |
| Scheduler | `EDU_RUN_EVERY` | Unsigned rollover-safe elapsed-time comparison |
| ADC10 | `edu_adc10_sample` | ADC10 registers, bounded polling |
| UART | `edu_uart_put`, `edu_uart_get`, `edu_uart_text` | USCI_A0 9600-baud driver |
| SPI | `edu_spi_begin`, `edu_spi_transfer` | USCI_B0 master + separate GPIO chip select |
| Software I²C | `edu_si2c_read_reg8`, `edu_si2c_write_reg8` | P2.0/P2.1 open-drain via direction toggling |
| Hardware I²C | `edu_i2c_begin`, `edu_i2c_write`, `edu_i2c_read`, `edu_i2c_read_reg8` | USCI_B0 master, bounded polling |

**A macro is best for a one-line operation. A small C function is best when an operation
needs sequencing, waits, status, error handling or state.** The functions contain ordinary,
heavily commented register C, not an RTOS or a second firmware framework.

## The included examples

Each `examples/0*/` folder is a separate, simple program that includes local copies of its
headers plus a CCS `.projectspec` import candidate:

1. `01-port-macros` — start with an LED, uncover register masks.
2. `02-uart-hello` — use the built-in Windows COM backchannel.
3. `03-adc-voltmeter` — read P1.4 as a 10-bit analog measurement.
4. `04-spi-jedec-id` — send an SPI command to a compatible 3.3V SPI memory.
5. `05-software-i2c-bus-scan` — discover devices without using USCI_B0.
6. `06-hardware-i2c-write` — send an I²C command over USCI_B0.
7. `07-scheduler` — share time without threads or task stacks.
8. `08-compare-macro-with-register` — watch register changes while debugging.

## No extra build system

- In **CCS**, choose `Project → Import CCS Projects` and the matching `.projectspec`
  where that version supports import, or create a C project with device `MSP430G2553`.
- Add the desired example's `main.c` and its same-folder headers to that project.
- Compile exactly **one** main program at a time. No extra libraries or linker script
  are supplied; CCS selects the device support for MSP430G2553.
- Use the correct debugger connection for original G2 or eZ-FET G2ET.
- See `docs/CCS_GUIDE.md` for pin wiring and qualification limitations.

**Conservative validation claim:** 8 examples passed Linux host GCC syntax-only checks with an
MSP430 *mock header*; 3 deterministic macro/scheduler/button tests passed. No actual TI compiler,
CCS import, logic analyzer, EnergyTrace, external sensor, or 3.3V LaunchPad hardware was
available for qualification in this build environment. Every bus transaction and pin-mux
change should be tested in CCS on real hardware before classroom deployment.

## Source and licensing

Piconomix contributed **architecture and educational inspiration**, not transplanted source
code; do not conflate its MIT license with this project's GPL-3.0 license. The surrounding
learning curriculum adapts the [ADolbyB exercises](https://github.com/ADolbyB/msp430g2553-labs)
under GPLv3. See `docs/ORIGINS.md`. Copying or adapting library code as part of that
curriculum retains its GPL-3.0 distribution obligations.
