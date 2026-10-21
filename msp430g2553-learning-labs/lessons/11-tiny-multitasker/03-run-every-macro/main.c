/*
 * LESSON 11 / STAGE 3 - THE FRIENDLY MACRO VIEW
 *
 * The behavior is the same as Stage 2. The scheduling logic is NOT
 * hidden in a runtime library: open tiny_scheduler.h to see exactly
 * how RUN_EVERY maps back into a C 'if' and an unsigned subtraction.
 *
 * Three cooperative jobs: LED (250ms), button (10ms), UART (1000ms).
 * A task must finish quickly and return, or other tasks run late.
 */
#include <stdint.h>
#include "edu430.h"
#include "tiny_scheduler.h"
#include "button_filter.h"

/* Timer_A provides a hardware tick. It is still an ordinary MSP430
 * register configuration, not an operating-system service.
 */
static volatile uint16_t ticks_ms = 0;
static unsigned int observed_presses = 0;
static button_filter_t button_state = {0u, 0u, 0u};

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a0_tick(void)
{
    ticks_ms++;
}

static void start_1ms_clock(void)
{
    TA0CCR0 = 999;                      /* 1MHz clock / 1000 = 1ms. */
    TA0CCTL0 = CCIE;                    /* Enable CCR0 interrupt. */
    TA0CTL = TASSEL_2 | MC_1 | TACLR;  /* SMCLK, UP, reset. */
    __enable_interrupt();
}

/* Now each job states WHAT it accomplishes, not WHEN to run. */
static void blink_led(void)
{
    LED_TOGGLE(LED_A);  /* P1OUT ^= BIT0; */
}

static void check_button(void)
{
    /* BUTTON_DOWN() expands to ((P1IN & BIT3) == 0).
     * Only count a new, debounced press, never a held button.
     */
    if (button_new_press(&button_state, BUTTON_DOWN() ? 1u : 0u))
    {
        observed_presses++;
        LED_TOGGLE(LED_B); /* Show each accepted button event. */
    }
}

static void report_events(void)
{
    serial_text("presses,");
    serial_uint(observed_presses);
    serial_text("\r\n");
}

int main(void)
{
    /* These little structs are the scheduler's entire task table:
     * last-run time, and how many milliseconds to wait.
     */
    periodic_job_t led_job    = {0u, 250u};
    periodic_job_t button_job = {0u, 10u};
    periodic_job_t report_job = {0u, 1000u};

    board_clock_1mhz();
    board_leds();
    board_button();
    board_uart_9600();
    start_1ms_clock();
    serial_text("STAGE 3: RUN_EVERY; open tiny_scheduler.h!\r\n");

    while (1)
    {
        /* uint16_t snapshot is atomic on this 16-bit MSP430.
         * No work occurs inside the timer interrupt beyond ticks_ms++.
         */
        uint16_t now = ticks_ms;
        RUN_EVERY(led_job,    now, blink_led);
        RUN_EVERY(button_job, now, check_button);
        RUN_EVERY(report_job, now, report_events);
    }
}
