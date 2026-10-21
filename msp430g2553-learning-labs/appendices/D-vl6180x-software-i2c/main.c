/* Advanced Appendix D: VL6180X time of flight using GPIO bit-banged I2C.
 * This is an original educational implementation of the I2C transaction
 * protocol, using mandatory tuning values documented by ST DT0037/AN4545.
 * Pins: SDA=P2.0, SCL=P2.1. Both NEED 4.7k pullups to 3.3 V.
 * Bare VL6180X IC is NOT a 3.3V part: use a breakout with regulator and
 * level shifting suitable for a 3.3V host. Check the breakout specifications.
 * A VL53L0X is a DIFFERENT device and cannot use these registers.
 * Status: experimental; requires actual-device validation.
 */
#include "edu430.h"
#define SDA BIT0
#define SCL BIT1
#define VL6180X_ADDR 0x29u  /* Seven-bit device address */
#define MAX_CLOCK_WAIT 2000u
static void bus_pause(void) { __delay_cycles(6); }
/* Open drain emulation: output low to assert zero, input to release line.
 * Never drive either wire high; external pull-ups provide ones.
 */
static void low(unsigned char mask) { CLEAR_BITS(P2OUT,mask); SET_BITS(P2DIR,mask); }
static void release(unsigned char mask) { CLEAR_BITS(P2DIR,mask); }
static unsigned char scl_release(void)
{
    unsigned int n;
    release(SCL);
    for (n=0; n<MAX_CLOCK_WAIT; ++n) {
        if (P2IN & SCL) return 1;  /* Permit clock stretching */
        bus_pause();
    }
    return 0;
}
static void i2c_init(void)
{
    CLEAR_BITS(P2SEL,SDA|SCL); CLEAR_BITS(P2SEL2,SDA|SCL);
    CLEAR_BITS(P2OUT,SDA|SCL); release(SDA|SCL);
}
static void i2c_start(void)
{
    release(SDA); (void)scl_release(); bus_pause();
    low(SDA); bus_pause(); low(SCL);
}
static void i2c_stop(void)
{
    low(SDA); bus_pause(); (void)scl_release(); bus_pause();
    release(SDA); bus_pause();
}
static unsigned char i2c_write(unsigned char byte)
{
    unsigned char mask;
    for (mask=0x80; mask; mask>>=1) {
        if (byte & mask) release(SDA); else low(SDA);
        bus_pause();
        if (!scl_release()) return 0;
        bus_pause(); low(SCL);
    }
    release(SDA); bus_pause();
    if (!scl_release()) return 0;
    mask = (unsigned char)((P2IN & SDA)==0);  /* Slave ACK is LOW */
    bus_pause(); low(SCL);
    return mask;
}
static unsigned char i2c_read(unsigned char nack)
{
    unsigned char result=0, i;
    release(SDA);
    for (i=0; i<8; ++i) {
        result <<= 1; bus_pause(); (void)scl_release();
        if (P2IN & SDA) result |= 1;
        bus_pause(); low(SCL);
    }
    if (nack) release(SDA); else low(SDA);
    bus_pause(); (void)scl_release(); bus_pause(); low(SCL);
    release(SDA);
    return result;
}
/* VL6180X has SIXTEEN-bit register addresses, not the usual 8-bit kind. */
static unsigned char write_reg(unsigned int reg, unsigned char value)
{
    unsigned char ok=1;
    i2c_start();
    ok &= i2c_write((VL6180X_ADDR<<1) | 0u);
    ok &= i2c_write((unsigned char)(reg>>8));
    ok &= i2c_write((unsigned char)reg);
    ok &= i2c_write(value);
    i2c_stop();
    return ok;
}
static unsigned char read_reg(unsigned int reg, unsigned char *value)
{
    unsigned char ok=1;
    i2c_start();
    ok &= i2c_write((VL6180X_ADDR<<1) | 0u);
    ok &= i2c_write((unsigned char)(reg>>8));
    ok &= i2c_write((unsigned char)reg);
    if (!ok) { i2c_stop(); return 0; }
    i2c_start();  /* Repeated START preserves register pointer */
    ok &= i2c_write((VL6180X_ADDR<<1) | 1u);
    if (ok) *value=i2c_read(1); /* NACK last received byte */
    i2c_stop();
    return ok;
}
struct RegisterSetting { unsigned int address; unsigned char value; };
/* Mandatory private settings from ST DT0037, column order normalized.
 * Applied ONLY when SYSTEM_FRESH_OUT_OF_RESET == 1.
 */
static const struct RegisterSetting tuning[] = {
 {0x0207,0x01},{0x0208,0x01},{0x0133,0x01},{0x0096,0x00},
 {0x0097,0xFD},{0x00E3,0x00},{0x00E4,0x04},{0x00E5,0x02},
 {0x00E6,0x01},{0x00E7,0x03},{0x00F5,0x02},{0x00D9,0x05},
 {0x00DB,0xCE},{0x00DC,0x03},{0x00DD,0xF8},{0x009F,0x00},
 {0x00A3,0x3C},{0x00B7,0x00},{0x00BB,0x3C},{0x00B2,0x09},
 {0x00CA,0x09},{0x0198,0x01},{0x01B0,0x17},{0x01AD,0x00},
 {0x00FF,0x05},{0x0100,0x05},{0x0199,0x05},{0x0109,0x07},
 {0x010A,0x30},{0x003F,0x46},{0x01A6,0x1B},{0x01AC,0x3E},
 {0x01A7,0x1F},{0x0103,0x01},{0x0030,0x00},{0x001B,0x0A},
 {0x003E,0x0A},{0x0131,0x04},{0x0011,0x10},{0x0014,0x24},
 {0x0031,0xFF},{0x00D2,0x01},{0x00F2,0x01}
};
static unsigned char sensor_init(void)
{
    unsigned int i;
    unsigned char model, fresh;
    if (!read_reg(0x0000,&model) || model != 0xB4) return 0;
    if (!read_reg(0x0016,&fresh)) return 0;
    if (fresh == 1) {
        for (i=0; i<sizeof tuning/sizeof tuning[0]; ++i)
            if (!write_reg(tuning[i].address,tuning[i].value)) return 0;
        if (!write_reg(0x0016,0x00)) return 0;
    }
    return 1;
}
/* Return 1 only for a completed, error-free range measurement. */
static unsigned char measure_mm(unsigned char *distance)
{
    unsigned int n;
    unsigned char state, error;
    if (!write_reg(0x0018,0x01)) return 0; /* SYSRANGE_START */
    for (n=0; n<400u; ++n) {
        if (!read_reg(0x004F,&state)) return 0;
        if ((state & 0x07u)==0x04u) break;
        __delay_cycles(1000);
    }
    if (n==400u) return 0;
    if (!read_reg(0x004D,&error)) return 0;
    if (!read_reg(0x0062,distance)) return 0;
    if (!write_reg(0x0015,0x07)) return 0;  /* Interrupt clear */
    return (error & 0xF0u)==0;             /* Upper nibble: range status */
}
int main(void)
{
    unsigned char mm;
    board_clock_1mhz(); board_uart_9600(); i2c_init();
    serial_text("VL6180X init\r\n");
    if (!sensor_init()) {serial_text("INIT_FAIL\r\n"); while(1) {}}
    serial_text("distance_mm\r\n");
    while (1) {
        if (measure_mm(&mm)) {serial_uint(mm); serial_text("\r\n");}
        else serial_text("RANGE_ERROR\r\n");
        __delay_cycles(200000);
    }
}
