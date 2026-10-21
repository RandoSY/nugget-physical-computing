/*
 * LESSON 11 / STAGE 2 - THREE COOPERATING JOBS, EXPLICIT C
 *
 * Same jobs as Stage 1, but none of the jobs contains a 250 ms delay:
 *   A. Toggle LED A every 250 ms.
 *   B. Sample/debounce the user button every 10 ms.
 *   C. Send a short progress report every 1000 ms.
 *
 * Timer_A0 is the ONLY clock interrupt. The interrupt does NOT run jobs.
 * Jobs run cooperatively in main(), one after another, and must return.
 *
 * To teach the underlying mechanism, P1OUT / P1IN and the elapsed-time
 * checks appear explicitly. Stage 3 renames this identical mechanism.
 */
#include <stdint.h>
#include "edu430.h"

/* A 16-bit millisecond counter takes only 2 bytes of RAM.
 * Since MSP430G2553 is a 16-bit processor, reading this 16-bit value
 * is atomic when correctly aligned. It wraps every ~65.536 seconds.
 * unsigned 16-bit subtraction makes small periods survive the wrap.
 * Never leave the main loop blocked for 65 seconds or longer.
 */
static volatile uint16_t ticks_ms = 0;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a0_tick(void)
{
    ticks_ms++;  /* Minimal ISR: one tick per timer period, NO serial here. */
}

static void start_timer_a_1ms(void)
{
    /* At 1 MHz SMCLK, 1000 ticks = 1 ms. In UP mode 0..999 is 1000. */
    TA0CCR0 = 999;
    TA0CCTL0 = CCIE;                      /* Compare-0 interrupt enabled. */
    TA0CTL = TASSEL_2 | MC_1 | TACLR;    /* SMCLK, count UP, reset timer. */
    __enable_interrupt();                  /* Allow interrupts globally. */
}

int main(void)
{
    uint16_t led_at = 0;
    uint16_t button_at = 0;
    uint16_t report_at = 0;
    unsigned int observed_presses = 0;
    unsigned char last_sample = 0;
    unsigned char stable_state = 0;
    unsigned char same_samples = 0;

    board_clock_1mhz();
    board_leds();
    board_button();
    board_uart_9600();
    start_timer_a_1ms();
    serial_text("STAGE 2: TIMER-DRIVEN COOPERATIVE JOBS\r\n");

    while (1)
    {
        /* The clock is written by the ISR. Reading 16 bits is atomic
         * on this target. All three jobs use the same snapshot.
         */
        uint16_t now = ticks_ms;

        /* JOB A: LED. The subtraction is converted back to uint16_t,
         * so a tick rollover still gives a sensible elapsed interval.
         */
        if ((uint16_t)(now - led_at) >= 250u)
        {
            led_at = now;
            P1OUT ^= BIT0;  /* Register-level version of LED_TOGGLE(LED_A). */
        }

        /* JOB B: sample input approximately every 10 ms.
         * Three equal samples (~30 ms) are required to accept a change.
         * Only a released -> pressed transition counts as a new press.
         */
        if ((uint16_t)(now - button_at) >= 10u)
        {
            unsigned char sample = ((P1IN & BIT3) == 0u) ? 1u : 0u;
            button_at = now;
            if (sample != last_sample)
            {
                last_sample = sample;
                same_samples = 1u;
            }
            else if (same_samples < 3u)
            {
                same_samples++;
            }
            if ((same_samples == 3u) && (sample != stable_state))
            {
                stable_state = sample;
                if (stable_state)
                {
                    observed_presses++;
                    P1OUT ^= BIT6;  /* LED B is an event indicator. */
                }
            }
        }

        /* JOB C: send a short CSV record once per second.
         * This serial helper waits for the UART transmitter, so it
         * blocks for a few milliseconds. Discuss this residual jitter.
         */
        if ((uint16_t)(now - report_at) >= 1000u)
        {
            report_at = now;
            serial_text("presses,");
            serial_uint(observed_presses);
            serial_text("\r\n");
        }
    }
}
