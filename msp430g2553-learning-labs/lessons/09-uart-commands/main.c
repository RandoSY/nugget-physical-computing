/* UART: receive and act
 * Learning goal: USCI_A0 polling receive
 * Hardware: USB backchannel, 9600 8N1
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   P1.1 is receive from the PC; UCA0RXBUF provides incoming bytes.
 *   A command is one ASCII character; no special library or parser.
 *   Reception is polled, which is easy to understand but not efficient.
 *   Test: map one new character onto a controlled GPIO action.
 */
#include "edu430.h"
int main(void)
{
    char command;
    board_clock_1mhz(); board_leds(); board_uart_9600();
    serial_text("Commands: A B O ?\r\n");
    while (1) {
        if (!(IFG2 & UCA0RXIFG)) continue; /* Nothing received yet */
        command = (char)UCA0RXBUF;           /* Reading clears RX flag */
        if (command == 'A' || command == 'a') {
            LED_TOGGLE(LED_A); serial_text("LED A toggled\r\n");
        } else if (command == 'B' || command == 'b') {
            LED_TOGGLE(LED_B); serial_text("LED B toggled\r\n");
        } else if (command == 'O' || command == 'o') {
            LED_OFF(LED_BOTH); serial_text("Both off\r\n");
        } else if (command == '?') {
            serial_text("A/B toggle, O off, ? help\r\n");
        }
    }
}
