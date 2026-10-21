/*
 * LESSON 11 / OPTIONAL ENERGYTRACE COMPARISON
 * MSP-EXP430G2ET (eZ-FET EnergyTrace), MSP430G2553.
 *
 * BUILD THE SAME PROGRAM TWICE and measure current/energy in CCS:
 *   IDLE_LOW_POWER = 0 : CPU spins in the main loop between jobs.
 *   IDLE_LOW_POWER = 1 : CPU sleeps in LPM0 until the next 1ms tick.
 *
 * IMPORTANT: SMCLK keeps running in LPM0, so Timer_A keeps ticking.
 * Timer ISR must clear LPM0 bits *on exit* to wake the main loop.
 * Ordinary interrupt entry alone is not enough to leave LPM0 awake.
 *
 * Fair-measurement switches:
 *   POWER_TEST_LED=0 removes heavy LED current (which can mask CPU gain).
 *   POWER_TEST_UART=0 prevents serial output skewing energy traces.
 * Set the same switches for both comparison builds.
 */
#include <stdint.h>
#include "edu430.h"
#include "tiny_scheduler.h"
#include "button_filter.h"

#define IDLE_LOW_POWER  1   /* Change to 0, rebuild, compare. */
#define POWER_TEST_LED  0   /* Set to 1 for visual demonstration. */
#define POWER_TEST_UART 0  /* Set to 1 for optional 9600-baud logging. */

static volatile uint16_t ticks_ms = 0;
static unsigned int observed_presses = 0;
static button_filter_t button_state = {0u, 0u, 0u};
static unsigned char logical_led = 0u;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a0_tick(void)
{
    ticks_ms++;
#if IDLE_LOW_POWER
    /* Change the saved SR so RETI returns to ACTIVE mode, not LPM0. */
    __bic_SR_register_on_exit(LPM0_bits);
#endif
}

static void start_1ms_clock(void)
{
    TA0CCR0 = 999;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | MC_1 | TACLR;
    __enable_interrupt();
}
static void blink_led(void)
{
    logical_led ^= 1u;
#if POWER_TEST_LED
    LED_TOGGLE(LED_A);
#else
    /* No physical LED current in either comparison build. */
    (void)logical_led;
#endif
}
static void check_button(void)
{
    if (button_new_press(&button_state, BUTTON_DOWN() ? 1u : 0u))
    {
        observed_presses++;
#if POWER_TEST_LED
        LED_TOGGLE(LED_B);
#endif
    }
}
static void report_events(void)
{
#if POWER_TEST_UART
    serial_text("presses,");
    serial_uint(observed_presses);
    serial_text("\r\n");
#else
    /* Explicitly retain the data, but omit expensive UART transmission. */
    (void)observed_presses;
#endif
}
int main(void)
{
    periodic_job_t led_job = {0u, 250u};
    periodic_job_t button_job = {0u, 10u};
    periodic_job_t report_job = {0u, 1000u};

    board_clock_1mhz();
    board_leds();
    board_button();
#if POWER_TEST_UART
    board_uart_9600();
#endif
    start_1ms_clock();

    while (1)
    {
        uint16_t now = ticks_ms;
        RUN_EVERY(led_job, now, blink_led);
        RUN_EVERY(button_job, now, check_button);
        RUN_EVERY(report_job, now, report_events);

#if IDLE_LOW_POWER
        /* Stop the CPU; leave SMCLK on to drive Timer_A.
         * GIE remains enabled so the timer interrupt can wake us.
         * There is a possible 1ms scheduling wake-up delay if a tick
         * arrives immediately before sleeping; that is acceptable here.
         */
        __bis_SR_register(LPM0_bits | GIE);
#else
        /* The CPU stays active and continuously checks the jobs. */
        __no_operation();
#endif
    }
}
