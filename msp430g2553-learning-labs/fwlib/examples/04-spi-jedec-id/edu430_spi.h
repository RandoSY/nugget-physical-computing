#ifndef EDU430_SPI_H
#define EDU430_SPI_H
#include <msp430.h>
#include <stdint.h>
/* Hardware USCI_B0 SPI, master mode.  Physical pins:
 * P1.5 CLK, P1.6 MISO (SOMI), P1.7 MOSI (SIMO).
 * WARNING: P1.6 also drives LED_B; SPI will take that pin away.
 * USCI_B0 cannot be simultaneously SPI and hardware I2C.
 * SPI modes: mode 0 = CPOL0, capture first edge -> UCCKPH in TI USCI.
 * The caller owns chip-select; choose an unrelated pin such as P2.0. */
#define EDU_SPI_MODE0 (UCCKPH)
#define EDU_SPI_MODE1 (0u)
#define EDU_SPI_MODE2 (UCCKPH | UCCKPL)
#define EDU_SPI_MODE3 (UCCKPL)
static inline void edu_spi_begin(uint8_t mode, uint8_t divider)
{
    if (divider == 0u) divider=2u;
    UCB0CTL1=UCSWRST | UCSSEL_2;
    UCB0CTL0=(uint8_t)(UCMST | UCSYNC | UCMSB | mode);
    UCB0BR0=divider; UCB0BR1=0u;
    P1SEL |= BIT5 | BIT6 | BIT7;
    P1SEL2 |= BIT5 | BIT6 | BIT7;
    UCB0CTL1 &= (uint8_t)~UCSWRST;
}
static inline uint8_t edu_spi_transfer(uint8_t tx, uint8_t *rx, uint32_t timeout)
{
    /* Read RXBUF first if RXIFG is set to discard stale data. */
    if (IFG2 & UCB0RXIFG) { volatile uint8_t discard=UCB0RXBUF; (void)discard; }
    while (!(IFG2 & UCB0TXIFG) && timeout) --timeout;
    if (!(IFG2 & UCB0TXIFG)) return 0u;
    UCB0TXBUF=tx;
    while (!(IFG2 & UCB0RXIFG) && timeout) --timeout;
    if (!(IFG2 & UCB0RXIFG)) return 0u;
    tx=UCB0RXBUF;
    if (rx) *rx=tx;
    return 1u;
}
#endif
