/* Historical comparison summary from Joel Brigida, OnBoard-LED-Pattern.c (2021).
 * Read the original unmodified file at:
 * https://github.com/ADolbyB/msp430g2553-labs/blob/main/C-Lang/LEDs/OnBoard-LED-Pattern.c
 * The complete original source remains in its upstream repository.
 * Key historical idioms: P1DIR |= BIT0 | BIT6; P1OUT ^= BIT0;
 * macro mapping: LED_TOGGLE(LED_A) -> P1OUT ^= BIT0;
 * This derivative educational project acknowledges the upstream GPLv3 license.
 */
