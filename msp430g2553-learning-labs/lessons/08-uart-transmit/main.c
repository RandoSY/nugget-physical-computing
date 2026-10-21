/* UART: publish measurements
 * Learning goal: USCI_A0 serial transmit
 * Hardware: USB backchannel, 9600 8N1
 * Built for MSP430G2553 G2/G2ET LaunchPad, Code Composer Studio.
 * Read the macros in edu430.h beside register-level C.
 */
/*
 * TEACHING NOTES — WHY THE REGISTERS MATTER
 * CONCEPT MAP
 *   UART and USB are not the same protocol. The LaunchPad programmer
 *   contains the bridge that makes MCU UART appear as a Windows COM port.
 *   P1.2 is UART transmit from MCU; UCA0TXBUF holds one data byte.
 *   We output simple CSV so spreadsheet software can analyze results.
 *   Test: record ten samples and predict the elapsed duration.
 */
#include "edu430.h"
int main(void)
{
    unsigned int count = 0;
    board_clock_1mhz(); board_leds(); board_uart_9600();
    serial_text("MSP430G2553 ready at 9600 baud\r\n");
    while (1) {
        serial_text("sample,");
        serial_uint(count++);
        serial_text("\r\n");
        LED_TOGGLE(LED_A);
        __delay_cycles(1000000);
    }
}
