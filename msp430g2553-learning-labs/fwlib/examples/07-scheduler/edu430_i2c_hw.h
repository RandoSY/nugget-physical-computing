#ifndef EDU430_I2C_HW_H
#define EDU430_I2C_HW_H
#include <msp430.h>
#include <stdint.h>
/* Hardware USCI_B0 I2C master, 7-bit addressing, single-master mode.
 * P1.6=SCL and P1.7=SDA. P1.6 conflicts with onboard LED_B.
 * USCI_B0 also hosts SPI: choose ONE mode at a time.
 * 1MHz SMCLK / 20 = 50 kHz, deliberately conservative for G2553 errata.
 * External pullups to 3.3V are mandatory; test with real hardware.
 * TX and RX return 1 on success, 0 for NACK or timeout.
 * This is a teaching driver, NOT full multi-master or DMA support.
 */
static inline void edu_i2c_begin(void)
{
    UCB0CTL1=UCSWRST | UCSSEL_2;
    UCB0CTL0=UCMST | UCMODE_3 | UCSYNC;
    UCB0BR0=20u; UCB0BR1=0u;
    P1SEL |= BIT6|BIT7; P1SEL2 |= BIT6|BIT7;
    UCB0CTL1 &= (uint8_t)~UCSWRST;
}
static inline uint8_t edu_i2c_wait_start(uint32_t timeout)
{
    /* Wait until START was physically placed on bus before loading TXBUF:
     * this also avoids the USCI34 erratum race. */
    while ((UCB0CTL1 & UCTXSTT) && !(UCB0STAT & UCNACKIFG) && timeout) --timeout;
    return (!(UCB0CTL1 & UCTXSTT) && !(UCB0STAT & UCNACKIFG));
}
static inline uint8_t edu_i2c_wait_tx(uint32_t timeout)
{
    while (!(IFG2 & UCB0TXIFG) && !(UCB0STAT & UCNACKIFG) && timeout) --timeout;
    return (IFG2 & UCB0TXIFG) && !(UCB0STAT & UCNACKIFG);
}
static inline uint8_t edu_i2c_wait_rx(uint32_t timeout)
{
    while (!(IFG2 & UCB0RXIFG) && !(UCB0STAT & UCNACKIFG) && timeout) --timeout;
    return (IFG2 & UCB0RXIFG) && !(UCB0STAT & UCNACKIFG);
}
static inline uint8_t edu_i2c_wait_stop(uint32_t timeout)
{
    while ((UCB0CTL1 & UCTXSTP) && timeout) --timeout;
    return (UCB0CTL1 & UCTXSTP)==0u;
}
static inline uint8_t edu_i2c_write(uint8_t addr7,const uint8_t *data,uint8_t len,uint32_t timeout)
{
    uint8_t i;
    if(addr7>0x7fu || (len && !data)) return 0u;
    if (UCB0STAT & UCBBUSY) return 0u;
    UCB0SA=addr7; UCB0STAT &= (uint8_t)~UCNACKIFG;
    UCB0CTL1 |= UCTR | UCTXSTT;
    if(!edu_i2c_wait_start(timeout)) goto fail;
    for(i=0;i<len;i++) {
        if(!edu_i2c_wait_tx(timeout)) goto fail;
        UCB0TXBUF=data[i];
    }
    if(!edu_i2c_wait_tx(timeout)) goto fail;
    UCB0CTL1 |= UCTXSTP;
    if(!edu_i2c_wait_stop(timeout)) return 0u;
    return !(UCB0STAT & UCNACKIFG);
fail:
    UCB0CTL1 |= UCTXSTP;
    (void)edu_i2c_wait_stop(timeout);
    return 0u;
}
/* Simplest received-data workflow: read N bytes from a device's current
 * pointer. STOP must be scheduled BEFORE the final RXBUF read on USCI_B.
 */
static inline uint8_t edu_i2c_read(uint8_t addr7,uint8_t *data,uint8_t len,uint32_t timeout)
{
    uint8_t i;
    if(addr7>0x7fu || !data || len==0u) return 0u;
    if(UCB0STAT & UCBBUSY) return 0u;
    UCB0SA=addr7; UCB0STAT &= (uint8_t)~UCNACKIFG;
    UCB0CTL1 &= (uint8_t)~UCTR;
    UCB0CTL1 |= UCTXSTT;
    if(!edu_i2c_wait_start(timeout)) goto fail;
    for(i=0;i<len;i++) {
        /* For single byte, STOP immediately after START drops.
         * For multibyte, STOP before reading next-to-last/last.
         */
        if(i==(uint8_t)(len-1u)) UCB0CTL1 |= UCTXSTP;
        if(!edu_i2c_wait_rx(timeout)) goto fail;
        data[i]=UCB0RXBUF;
    }
    return edu_i2c_wait_stop(timeout) && !(UCB0STAT & UCNACKIFG);
fail:
    UCB0CTL1 |= UCTXSTP;
    (void)edu_i2c_wait_stop(timeout);
    return 0u;
}

/* Read a single 8-bit register through a REPEATED START transaction:
 * [START + addr-W + register] [RESTART + addr-R + one-byte] [STOP]
 * Important: delay STOP until RX START has completed, but issue STOP
 * before the first/only received byte enters RXBUF (USCI_B behavior).
 * Many ST sensors use 16-bit registers; use software-I2C's dedicated
 * 16-bit variant or extend the write phase accordingly for those devices.
 */
static inline uint8_t edu_i2c_read_reg8(uint8_t addr7,uint8_t reg,uint8_t *value,uint32_t timeout)
{
    if (addr7>0x7fu || !value || (UCB0STAT & UCBBUSY)) return 0u;
    UCB0SA=addr7;
    UCB0STAT &= (uint8_t)~UCNACKIFG;
    UCB0CTL1 |= UCTR | UCTXSTT;
    if (!edu_i2c_wait_start(timeout) || !edu_i2c_wait_tx(timeout)) goto fail;
    UCB0TXBUF=reg;
    if (!edu_i2c_wait_tx(timeout)) goto fail;
    UCB0CTL1 &= (uint8_t)~UCTR;
    UCB0CTL1 |= UCTXSTT;
    if (!edu_i2c_wait_start(timeout)) goto fail;
    UCB0CTL1 |= UCTXSTP;
    if (!edu_i2c_wait_rx(timeout)) goto fail;
    *value=UCB0RXBUF;
    return edu_i2c_wait_stop(timeout) && !(UCB0STAT & UCNACKIFG);
fail:
    UCB0CTL1 |= UCTXSTP;
    (void)edu_i2c_wait_stop(timeout);
    return 0u;
}

#endif
