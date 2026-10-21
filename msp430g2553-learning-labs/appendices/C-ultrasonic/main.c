/* HC-SR04 ultrasonic travel time
 * Measurement appendix, requires external sensor and validation.
 * Wiring: HC-SR04: 5 V VCC, GND common, TRIG=P2.3, ECHO via 10k/18k divider to P2.4.
 */
#include "edu430.h"
#define TRIGGER BIT3  /* Port 2, pin 3, MSP430 -> HC-SR04 */
#define ECHO    BIT4  /* Port 2, pin 4, voltage-divided 5 V echo */
/* Timer1_A counts SMCLK cycles: at 1 MHz, each timer tick = 1 us. */
static void sonar_init(void)
{
    CLEAR_BITS(P2OUT, TRIGGER);
    SET_BITS(P2DIR, TRIGGER);
    CLEAR_BITS(P2DIR, ECHO);
    TA1CTL = TASSEL_2 | MC_2 | TACLR;  /* Timer1_A continuous */
}
/* Return 0 on timeout (unmeasurable / no echo).
 * All timing differences use 16-bit unsigned arithmetic (wrap-safe
 * provided an interval stays below one 65536-us timer wrap).
 */
static unsigned int ping_us(void)
{
    unsigned int start, width;
    /* Ensure an old, stuck-high echo is not interpreted as a new one. */
    start = TA1R;
    while (P2IN & ECHO) {
        if ((unsigned int)(TA1R - start) >= 30000u) return 0;
    }
    SET_BITS(P2OUT, TRIGGER);
    __delay_cycles(12);              /* Datasheet minimum trigger 10 us */
    CLEAR_BITS(P2OUT, TRIGGER);
    start = TA1R;
    while (!(P2IN & ECHO)) {
        if ((unsigned int)(TA1R - start) >= 30000u) return 0;
    }
    start = TA1R;
    while (P2IN & ECHO) {
        if ((unsigned int)(TA1R - start) >= 30000u) return 0;
    }
    width = (unsigned int)(TA1R - start);
    return width;
}
int main(void)
{
    unsigned int microseconds, centimeters;
    board_clock_1mhz(); board_uart_9600(); sonar_init();
    serial_text("echo_us,distance_cm\r\n");
    while (1) {
        microseconds = ping_us();
        if (microseconds) {
            centimeters = (microseconds + 29u) / 58u; /* Approx 20 C air */
            serial_uint(microseconds); serial_char(',');
            serial_uint(centimeters); serial_text("\r\n");
        } else serial_text("NO_ECHO,\r\n");
        __delay_cycles(100000);      /* >=60 ms between pings */
    }
}
