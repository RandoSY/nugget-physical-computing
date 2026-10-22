#ifndef EDU430_I2C_REG16_H
#define EDU430_I2C_REG16_H
/* Edu430 hardware I2C bus transactions for MSP430G2553 (USCI_B0).
 *
 * Why does this exist? The original Edu430 v0.2 bus primitives support a
 * one-byte register address. ST's VL6180X has TWO-byte register addresses.
 * This layer adds generic START/REPEATED START/STOP transactions so our
 * optical sensor is a first-class example of a transferable protocol.
 *
 * Electrical: P1.6 = UCB0SCL, P1.7 = UCB0SDA, BOTH pullups to 3.3 V.
 * Remove LED2 jumper (P1.6) if fitted: the LED loads the bus.
 * USCI_B0 cannot operate in SPI mode at the same time.
 *
 * Constraints: 7-bit single-master, polling, 1 MHz SMCLK / 20 = 50 kHz.
 * The timeout argument is a bounded software spin COUNT, NOT milliseconds.
 * This version's one-byte reads are the primary qualification target.
 * Multiple received bytes need verification against the G2553 USCI30 erratum.
 * No interrupt-driven transfer, DMA, multi-master arbitration, or clock
 * recovery is claimed. Actual board qualification remains outstanding.
 *
 * TI references: SLAU144 (x2xx USCI I2C), SLAA382A, SLAZ440 (errata).
 */
#include <msp430.h>
#include <stdint.h>

/* A real error value, not merely success/failure, is invaluable for labs. */
typedef enum {
    EDU_I2C_OK = 0,
    EDU_I2C_ARGUMENT = 1,
    EDU_I2C_BUS_BUSY = 2,
    EDU_I2C_NACK = 3,
    EDU_I2C_TIMEOUT = 4
} edu_i2c_status_t;

/* Configure the microcontroller's I2C hardware. SMCLK is assumed 1 MHz. */
static inline void edu_i2c_hw_begin_50khz(void)
{
    UCB0CTL1 = UCSWRST | UCSSEL_2;
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0BR0 = 20u; UCB0BR1 = 0u;
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;
    UCB0CTL1 &= (uint8_t)~UCSWRST;
}

/* Wait for a control bit to CLEAR, or for a NACK. A status code is returned. */
static inline edu_i2c_status_t edu_i2c_hw_control_clear(uint8_t flag,
                                                          uint16_t limit)
{
    while ((UCB0CTL1 & flag) && limit && !(UCB0STAT & UCNACKIFG))
        --limit;
    if (UCB0STAT & UCNACKIFG) return EDU_I2C_NACK;
    return (UCB0CTL1 & flag) ? EDU_I2C_TIMEOUT : EDU_I2C_OK;
}

/* Wait for a USCI data flag to SET, or for a NACK. */
static inline edu_i2c_status_t edu_i2c_hw_data_ready(uint8_t flag,
                                                       uint16_t limit)
{
    while (!(IFG2 & flag) && limit && !(UCB0STAT & UCNACKIFG))
        --limit;
    if (UCB0STAT & UCNACKIFG) return EDU_I2C_NACK;
    return (IFG2 & flag) ? EDU_I2C_OK : EDU_I2C_TIMEOUT;
}

/* One conceptual I2C transaction:
 *  START + 7bit-address/W + write_bytes
 *  [REPEATED START + same-address/R + read_bytes]
 *  STOP
 *
 * A read of ST register 0x0062 therefore sends TWO bytes (0x00,0x62),
 * then changes direction without issuing an intervening STOP.
 *
 * For rx_count==1, STOP is requested immediately after the receive START
 * completes, BEFORE reading RXBUF (essential MSP430 USCI behavior).
 * For rx_count>1, STOP is armed for the last byte; this multi-byte branch
 * is provisional owing to device-specific USCI30 timing considerations.
 *
 * The caller can observe NACK vs timeout vs a busy bus.
 */
static inline edu_i2c_status_t edu_i2c_hw_transfer(uint8_t address7,
                                                   const uint8_t *tx, uint8_t tx_count,
                                                   uint8_t *rx, uint8_t rx_count,
                                                   uint16_t limit)
{
    uint8_t i;
    edu_i2c_status_t result;
    if (address7 > 0x7fu || (!tx_count && !rx_count) ||
        (tx_count && !tx) || (rx_count && !rx) || !limit)
        return EDU_I2C_ARGUMENT;
    if (UCB0STAT & UCBBUSY) return EDU_I2C_BUS_BUSY;

    UCB0SA = address7;                        /* 7-bit device address. */
    UCB0STAT &= (uint8_t)~UCNACKIFG;          /* Discard previous NACK. */

    if (tx_count) {
        UCB0CTL1 |= UCTR | UCTXSTT;          /* Transmitter + START. */
        result = edu_i2c_hw_control_clear(UCTXSTT, limit);
        if (result != EDU_I2C_OK) goto fail;
        for (i=0u; i<tx_count; ++i) {
            result = edu_i2c_hw_data_ready(UCB0TXIFG, limit);
            if (result != EDU_I2C_OK) goto fail;
            UCB0TXBUF = tx[i];               /* Hardware clocks each byte. */
        }
        result = edu_i2c_hw_data_ready(UCB0TXIFG, limit);
        if (result != EDU_I2C_OK) goto fail;
    }

    if (rx_count) {
        UCB0CTL1 &= (uint8_t)~UCTR;          /* Become master receiver. */
        UCB0CTL1 |= UCTXSTT;                 /* REPEATED START if TX preceded. */
        result = edu_i2c_hw_control_clear(UCTXSTT, limit);
        if (result != EDU_I2C_OK) goto fail;
        for (i=0u; i<rx_count; ++i) {
            if (i == (uint8_t)(rx_count-1u))
                UCB0CTL1 |= UCTXSTP;         /* NACK final byte + STOP. */
            result = edu_i2c_hw_data_ready(UCB0RXIFG, limit);
            if (result != EDU_I2C_OK) goto fail;
            rx[i] = (uint8_t)UCB0RXBUF;       /* Read clears RXIFG. */
        }
    } else {
        UCB0CTL1 |= UCTXSTP;                 /* Write-only transaction STOP. */
    }

    result = edu_i2c_hw_control_clear(UCTXSTP, limit);
    if (result != EDU_I2C_OK) return result;
    return EDU_I2C_OK;

fail:
    /* Best-effort STOP; a held-low wire still needs hardware diagnosis. */
    UCB0CTL1 |= UCTXSTP;
    (void)edu_i2c_hw_control_clear(UCTXSTP, limit);
    return result;
}

/* Convenience operations follow from the SAME transaction primitive. */
static inline edu_i2c_status_t edu_i2c_hw_write_reg16(uint8_t slave,
                                    uint16_t reg, uint8_t value, uint16_t limit)
{
    uint8_t message[3];
    message[0] = (uint8_t)(reg >> 8);  /* High register-address byte FIRST. */
    message[1] = (uint8_t)reg;
    message[2] = value;
    return edu_i2c_hw_transfer(slave, message, 3u, 0, 0u, limit);
}
static inline edu_i2c_status_t edu_i2c_hw_read_reg16(uint8_t slave,
                                    uint16_t reg, uint8_t *value, uint16_t limit)
{
    uint8_t addr[2];
    if (!value) return EDU_I2C_ARGUMENT;
    addr[0] = (uint8_t)(reg >> 8);
    addr[1] = (uint8_t)reg;
    return edu_i2c_hw_transfer(slave, addr, 2u, value, 1u, limit);
}
/* More ordinary sensors have 8-bit register addresses. Same engine. */
static inline edu_i2c_status_t edu_i2c_hw_read_reg8(uint8_t slave,
                                   uint8_t reg, uint8_t *value, uint16_t limit)
{
    return edu_i2c_hw_transfer(slave, &reg, 1u, value, 1u, limit);
}
#endif
