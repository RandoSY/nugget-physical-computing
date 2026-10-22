#ifndef EDU430_UART_H
#define EDU430_UART_H
#include <msp430.h>
#include <stdint.h>
/* UART backchannel: P1.1 RX, P1.2 TX; 9600-8N1 from 1MHz SMCLK.
 * Both LaunchPad generations route these through jumpers to USB debugger.
 * Blocking, bounded polling for beginner telemetry; NOT ISR-safe. */
static inline void edu_uart_9600_init(void)
{
    UCA0CTL1 = UCSWRST | UCSSEL_2;  /* keep peripheral reset during setup */
    P1SEL |= BIT1 | BIT2; P1SEL2 |= BIT1 | BIT2;
    UCA0BR0=104u; UCA0BR1=0u; UCA0MCTL=UCBRS0;
    UCA0CTL1 &= (uint8_t)~UCSWRST;
}
static inline uint8_t edu_uart_put(uint8_t byte, uint32_t timeout)
{
    while (!(IFG2 & UCA0TXIFG) && timeout) --timeout;
    if (!(IFG2 & UCA0TXIFG)) return 0u;
    UCA0TXBUF=byte; return 1u;
}
static inline uint8_t edu_uart_get(uint8_t *byte)
{
    if (!(IFG2 & UCA0RXIFG) || !byte) return 0u;
    *byte=UCA0RXBUF; return 1u;
}
static inline uint8_t edu_uart_text(const char *s, uint32_t timeout)
{
    if (!s) return 0u;
    while (*s) { if (!edu_uart_put((uint8_t)*s++,timeout)) return 0u; }
    return 1u;
}
#define EDU_UART_RX_READY() (IFG2 & UCA0RXIFG)
#define EDU_UART_TX_READY() (IFG2 & UCA0TXIFG)
#endif
