#ifndef EDU430_I2C_SOFT_H
#define EDU430_I2C_SOFT_H
#include <msp430.h>
#include <stdint.h>
/* Open-drain software I2C using P2.0 SCL and P2.1 SDA (G2/G2ET).
 * ALWAYS use external pullups to 3.3V (typically 4.7 kOhm).
 * NEVER drive an I2C wire actively high: release it as INPUT instead.
 * Educational 7-bit-address, single-master driver; no multi-master arbitration.
 * Configurable timing depends on CPU clock and instruction overhead.
 * Sensor must share GND and tolerate the 3.3V bus.
 */
#define EDU_SOFT_SCL BIT0
#define EDU_SOFT_SDA BIT1
static inline void edu_si2c_delay(void) { __delay_cycles(5); }
static inline void edu_si2c_scl_low(void){P2OUT &= (uint8_t)~EDU_SOFT_SCL; P2DIR |= EDU_SOFT_SCL;}
static inline void edu_si2c_scl_release(void){P2DIR &= (uint8_t)~EDU_SOFT_SCL;}
static inline void edu_si2c_sda_low(void){P2OUT &= (uint8_t)~EDU_SOFT_SDA; P2DIR |= EDU_SOFT_SDA;}
static inline void edu_si2c_sda_release(void){P2DIR &= (uint8_t)~EDU_SOFT_SDA;}
static inline uint8_t edu_si2c_scl_high(uint16_t limit)
{
    edu_si2c_scl_release(); /* permit slave clock-stretching */
    while (!(P2IN & EDU_SOFT_SCL) && limit) --limit;
    return (P2IN & EDU_SOFT_SCL) != 0u;
}
static inline uint8_t edu_si2c_init(void)
{
    P2SEL &= (uint8_t)~(EDU_SOFT_SCL | EDU_SOFT_SDA);
    P2SEL2 &= (uint8_t)~(EDU_SOFT_SCL | EDU_SOFT_SDA);
    P2REN &= (uint8_t)~(EDU_SOFT_SCL | EDU_SOFT_SDA);
    P2OUT &= (uint8_t)~(EDU_SOFT_SCL | EDU_SOFT_SDA);
    P2DIR &= (uint8_t)~(EDU_SOFT_SCL | EDU_SOFT_SDA);
    edu_si2c_delay();
    return (P2IN & (EDU_SOFT_SCL | EDU_SOFT_SDA)) == (EDU_SOFT_SCL | EDU_SOFT_SDA);
}
static inline uint8_t edu_si2c_start(void)
{
    edu_si2c_sda_release();
    if (!edu_si2c_scl_high(1000u)) return 0u;
    edu_si2c_delay();
    if (!(P2IN & EDU_SOFT_SDA)) return 0u; /* SDA stuck low */
    edu_si2c_sda_low(); edu_si2c_delay(); edu_si2c_scl_low();
    return 1u;
}
/* Repeated START: SDA has been released after the preceding byte. */
static inline uint8_t edu_si2c_restart(void)
{
    edu_si2c_sda_release(); edu_si2c_delay();
    return edu_si2c_start();
}
static inline void edu_si2c_stop(void)
{
    edu_si2c_sda_low(); edu_si2c_delay();
    (void)edu_si2c_scl_high(1000u); edu_si2c_delay();
    edu_si2c_sda_release(); edu_si2c_delay();
}
static inline uint8_t edu_si2c_bit(uint8_t value)
{
    if (value) edu_si2c_sda_release(); else edu_si2c_sda_low();
    edu_si2c_delay();
    if (!edu_si2c_scl_high(1000u)) return 0u;
    edu_si2c_delay(); edu_si2c_scl_low();
    return 1u;
}
static inline uint8_t edu_si2c_write(uint8_t value)
{
    uint8_t i,ack;
    for (i=0;i<8;i++) { if (!edu_si2c_bit(value & 0x80u)) return 0u; value<<=1; }
    edu_si2c_sda_release(); edu_si2c_delay();
    if (!edu_si2c_scl_high(1000u)) return 0u;
    ack=(P2IN & EDU_SOFT_SDA)==0u; /* ACK = slave pulls SDA low */
    edu_si2c_delay(); edu_si2c_scl_low();
    return ack;
}
static inline uint8_t edu_si2c_read(uint8_t *value,uint8_t ack)
{
    uint8_t i,v=0;
    if (!value) return 0u;
    edu_si2c_sda_release();
    for(i=0;i<8;i++) {
        v<<=1; edu_si2c_delay();
        if (!edu_si2c_scl_high(1000u)) return 0u;
        if (P2IN & EDU_SOFT_SDA) v|=1u;
        edu_si2c_delay(); edu_si2c_scl_low();
    }
    *value=v;
    return edu_si2c_bit(ack ? 0u : 1u); /* ACK=0, NACK=1 */
}
static inline uint8_t edu_si2c_write_reg8(uint8_t addr7,uint8_t reg,uint8_t value)
{
    uint8_t ok;
    if(addr7>0x7fu || !edu_si2c_start()) return 0u;
    ok=edu_si2c_write((uint8_t)(addr7<<1)) &&
       edu_si2c_write(reg) && edu_si2c_write(value);
    edu_si2c_stop(); return ok;
}
static inline uint8_t edu_si2c_read_reg8(uint8_t addr7,uint8_t reg,uint8_t *out)
{
    uint8_t ok;
    if(!out || addr7>0x7fu || !edu_si2c_start()) return 0u;
    ok=edu_si2c_write((uint8_t)(addr7<<1)) && edu_si2c_write(reg);
    if(ok) ok=edu_si2c_restart() && edu_si2c_write((uint8_t)((addr7<<1)|1u));
    if(ok) ok=edu_si2c_read(out,0u);
    edu_si2c_stop(); return ok;
}
/* Bus recovery is optional: 9 SCL clocks, then STOP.
 * This assumes no other master is using the bus. */
static inline void edu_si2c_recover(void)
{
    uint8_t i;
    edu_si2c_sda_release();
    for(i=0;i<9u;i++) {edu_si2c_scl_low(); edu_si2c_delay(); (void)edu_si2c_scl_high(1000u); edu_si2c_delay();}
    edu_si2c_stop();
}
#endif
