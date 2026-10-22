/* Edu430 inquiry exemplar 09 — optical distance with ST VL6180X.
 *
 * This is a stand-alone Code Composer Studio MSP430G2553 project.
 * Wiring: VL6180X **3.3V-logic-compatible breakout**:
 *       SCL -> P1.6 / UCB0SCL  (remove LED2 jumper)
 *       SDA -> P1.7 / UCB0SDA
 *       GND -> LaunchPad GND
 *       VIN -> breakout rated supply (check module, not bare silicon)
 *       4.7k pullups from SCL and SDA to 3.3V if not on breakout
 *       USB -> PC provides 9600-8N1 serial reporting on P1.1/P1.2.
 *
 * WARNING: Bare VL6180X silicon requires lower-voltage supplies/levels.
 * This example is NOT for VL53L0X, VL53L1X, or VL53L4CD.
 *
 * What we have deliberately exposed to students:
 *    SENSOR:       edu_vl6180x_range_mm(&bus, &distance, 200)
 *    TRANSACTION:  edu_i2c_hw_read_reg16(0x29, 0x0062, &distance,...)
 *    REGISTER:     UCB0SA, UCTXSTT, UCB0TXBUF, UCB0RXBUF, UCTXSTP
 *    ELECTRICITY:  SCL/SDA LOW pulse sequences and 3.3V pullups.
 *
 * This is bench-UNVERIFIED; compile with the installed TI toolchain,
 * verify the wiring, then observe ACK, bus waveforms, model ID and ranges.
 */
#include <msp430.h>
#include <stdint.h>
#include "edu430_system.h"
#include "edu430_uart.h"
#include "edu430_i2c_reg16.h"
#include "edu430_vl6180x.h"

/* Bounded software polling per I2C status test, not milliseconds. */
#define BUS_SPIN_LIMIT 60000u

/* Adapter connects the generic optical sensor driver to real USCI_B0. */
static uint8_t board_read(void *unused, uint16_t reg, uint8_t *byte)
{
    (void)unused;
    return edu_i2c_hw_read_reg16(EDU_VL6180X_ADDR7, reg,
                                  byte, BUS_SPIN_LIMIT) == EDU_I2C_OK;
}
static uint8_t board_write(void *unused, uint16_t reg, uint8_t byte)
{
    (void)unused;
    return edu_i2c_hw_write_reg16(EDU_VL6180X_ADDR7, reg,
                                   byte, BUS_SPIN_LIMIT) == EDU_I2C_OK;
}
static void board_wait_1ms(void *unused)
{
    (void)unused;
    __delay_cycles(1000); /* 1 MHz calibrated MCLK: ~1 ms. */
}
static const edu_vl6180x_bus_t bus = {
    board_read, board_write, board_wait_1ms, 0
};

/* Unlike full printf(), three decimal digits do not need a large library.
 * The VL6180X result itself is 8 bits (0 to 255, although its useful
 * specified range is much shorter). Output is line-oriented CSV.
 */
static void send_mm(uint8_t mm)
{
    if (mm >= 100u) (void)edu_uart_put('0' + mm/100u, BUS_SPIN_LIMIT);
    if (mm >= 10u)  (void)edu_uart_put('0' + (mm/10u)%10u, BUS_SPIN_LIMIT);
    (void)edu_uart_put('0' + mm%10u, BUS_SPIN_LIMIT);
    (void)edu_uart_text("\r\n", BUS_SPIN_LIMIT);
}

int main(void)
{
    edu_vl6180x_status_t result;
    uint8_t millimeters;
    EDU_WATCHDOG_STOP();
    if (!edu_clock_1mhz()) for (;;) { /* missing DCO calibration */ }
    edu_uart_9600_init();
    edu_i2c_hw_begin_50khz();
    (void)edu_uart_text("VL6180X,MSP430G2553,hardware_I2C\r\n",
                        BUS_SPIN_LIMIT);
    /* The optical part must have completed its power-on reset first. */
    __delay_cycles(5000);
    result = edu_vl6180x_initialize(&bus);
    if (result != EDU_VL_OK) {
        (void)edu_uart_text("INIT_ERROR: check power, address 0x29, "
                            "3.3V pullups, and LED2 jumper\r\n",
                            BUS_SPIN_LIMIT);
        for (;;) { }
    }
    (void)edu_uart_text("distance_mm\r\n", BUS_SPIN_LIMIT);
    for (;;) {
        result = edu_vl6180x_range_mm(&bus, &millimeters, 200u);
        if (result == EDU_VL_OK) send_mm(millimeters);
        else if (result == EDU_VL_NOT_READY)
            (void)edu_uart_text("ERR_TIMEOUT\r\n", BUS_SPIN_LIMIT);
        else if (result == EDU_VL_RANGE_ERROR)
            (void)edu_uart_text("ERR_RANGE\r\n", BUS_SPIN_LIMIT);
        else (void)edu_uart_text("ERR_I2C\r\n", BUS_SPIN_LIMIT);
        __delay_cycles(150000); /* ~150ms between measurements. */
    }
}
