/*
 * LESSON 11 / STAGE 1 - BLOCKING PROGRAM
 * MSP430G2553, both original G2 and newer G2ET LaunchPads.
 *
 * EXPERIMENT: What happens if a student taps the button while
 *             the processor is inside a 250 ms busy-wait?
 *
 * Three jobs are in this one sequential loop:
 *   A: Toggle LED A (P1.0) once every nominal 250 milliseconds.
 *   B: Watch the button on P1.3 and count observed new presses.
 *   C: Send the observed number of presses every four iterations
 *      using the LaunchPad USB backchannel UART at 9600 baud.
 *
 * This is intentionally NOT a reliable multitasker. It teaches why
 * blocking delays compromise input response and timing accuracy.
 * The UART itself is also a blocking operation.
 */
#include "edu430.h"

#define BUSY_DELAY_MS_250() __delay_cycles(250000UL)

int main(void)
{
    unsigned int observed_presses = 0;
    unsigned int quarters = 0;
    unsigned char button_was_down = 0;

    board_clock_1mhz();  /* Stop watchdog; configure factory-calibrated 1 MHz clock. */
    board_leds();        /* LED A is P1.0, LED B is P1.6, regardless of color. */
    board_button();      /* P1.3 is input with internal pull-up; pressed reads 0. */
    board_uart_9600();   /* P1.1 / P1.2 are UART RX / TX. */

    serial_text("STAGE 1: BLOCKING. Short taps may be missed.\r\n");

    while (1)
    {
        /* Traditional bit operation hidden by an educational macro:
         * LED_TOGGLE(LED_A) becomes P1OUT ^= BIT0;
         */
        LED_TOGGLE(LED_A);

        /* CPU cannot service the other jobs during this wait.
         * __delay_cycles counts clock cycles, not milliseconds;
         * 250,000 cycles at 1 MHz is approximately 250 ms.
         */
        BUSY_DELAY_MS_250();

        /* A brief button press can happen completely inside the delay.
         * This test therefore misses events. There is no debounce here.
         */
        if (BUTTON_DOWN() && !button_was_down)
        {
            observed_presses++;
        }
        button_was_down = BUTTON_DOWN() ? 1u : 0u;

        quarters++;
        if (quarters == 4u)
        {
            quarters = 0;
            serial_text("presses,");
            serial_uint(observed_presses);
            serial_text("\r\n");
        }
    }
}
