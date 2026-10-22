# Macro Atlas: a reference teachers can put beside the oscilloscope

| A novice writes | Peel back the layer | Physical effect |
|---|---|---|
| `EDU_PIN_ON(P1,BIT0)` | `P1OUT |= BIT0` | Drives P1.0 high |
| `EDU_GPIO_OUTPUT(P1,BIT0)` | `P1SEL &= ~BIT0; P1SEL2 &= ~BIT0; P1DIR |= BIT0` | Enables GPIO output |
| `EDU_GPIO_PULLUP(P1,BIT3)` | Set `P1OUT` and `P1REN` bits, clear `P1DIR` | Button rests HIGH |
| `EDU_PIN_TOGGLE(P1,BIT0)` | `P1OUT ^= BIT0` | Flip output latch |
| `EDU_GPIO_IRQ_FALLING(P1,BIT3)` | `P1IES |= BIT3` | Prepare falling-edge interrupt |
| `EDU_TIMER_1MS_TA0()` | `TA0CCR0=999; TA0CCTL0=CCIE; ...` | 1ms periodic IRQ at 1MHz SMCLK |
| `EDU_RUN_EVERY(job,now,toggle)` | `if (edu_job_due(...)) toggle()` | Cooperative task dispatch |
| `edu_adc10_sample(&raw,t)` | `ADC10SC`, poll `ADC10BUSY`, read `ADC10MEM` | Converts voltage to counts |
| `edu_pwm_ta0_p16_begin(500)` | `TA0CCR0=999; TA0CCR1=500; ...` | 50% PWM at 1kHz on P1.6 |
| `edu_uart_put('H',t)` | Check `UCA0TXIFG`; write `UCA0TXBUF` | Send serial byte |
| `edu_spi_transfer(0x9F,&rx,t)` | `UCB0TXBUF=...`, wait `UCB0RXIFG` | Clock SPI byte |
| `edu_i2c_write(...)` | `UCB0SA`, `UCTR`, `UCTXSTT`, `UCTXSTP` | Address/write/STOP |
| `edu_si2c_read_reg8(...)` | Drive low or release P2 DIR, read P2IN | Open-drain bus transaction |

### Why some features are functions, not macros

A `SPI_TRANSFER()` macro that secretly blocks forever, repeats its argument,
or overwrites another peripheral configuration makes learning harder and firmware
less reliable. Use short macros for genuinely simple register modifications.
Use short, named C functions for operations with multiple steps or failure modes.

### Migrating from old `msp430_easy.h`

| Historical MSP430 macro | Equivalent Edu430 concept |
|---|---|
| `MSP_GPIO_OUTPUT_INIT_LOW(MSP_P1_0)` | `EDU_GPIO_OUTPUT_LOW(P1,BIT0)` |
| `MSP_GPIO_OUT_TOGGLE(MSP_P1_0)` | `EDU_PIN_TOGGLE(P1,BIT0)` |
| `MSP_UART_A0_TX_READY()` | `EDU_UART_TX_READY()` |
| `MSP_TIMER_A0_START_ACLK_UP(n)` | `EDU_TIMER_UP(TA0,n,TASSEL_1 \| ID_0)` |
| `MSP_ADC10_READ_BLOCKING(result)` | `edu_adc10_sample(&result,timeout)` |
| `MSP_ENTER_LPM0_WITH_IRQS()` | `EDU_SLEEP_LPM0()` |

**Difference of intent:** The older manual's macros are kept as historical
reference. Edu430 uses shorter general names with a consistent `EDU_` prefix,
and it introduces bounded waits and status returns for I/O.
