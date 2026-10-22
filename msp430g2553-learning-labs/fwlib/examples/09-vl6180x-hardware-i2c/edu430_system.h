#ifndef EDU430_SYSTEM_H
#define EDU430_SYSTEM_H
#include <msp430.h>
#include <stdint.h>
/* Begin with the smallest useful clock configuration: calibrated 1 MHz DCO.
 * The return value tells the caller whether calibration is available.
 * Register-level translation: DCOCTL=0; BCSCTL1=CALBC1_1MHZ;
 *                              DCOCTL=CALDCO_1MHZ; */
static inline uint8_t edu_clock_1mhz(void)
{
    if (CALBC1_1MHZ == 0xFFu || CALDCO_1MHZ == 0xFFu) return 0u;
    DCOCTL=0; BCSCTL1=CALBC1_1MHZ; DCOCTL=CALDCO_1MHZ;
    BCSCTL2=0;  /* MCLK and SMCLK from DCO / 1, no divider. */
    return 1u;
}
#define EDU_WATCHDOG_STOP() (WDTCTL = WDTPW | WDTHOLD)
/* Caller supplies watchdog configuration to avoid silently changing modes. */
#define EDU_WATCHDOG_KICK(config) (WDTCTL = WDTPW | WDTCNTCL | (config))
#define EDU_IRQ_ON() __enable_interrupt()
#define EDU_IRQ_OFF() __disable_interrupt()
#define EDU_SLEEP_LPM0() __bis_SR_register(LPM0_bits | GIE)
#define EDU_SLEEP_LPM3() __bis_SR_register(LPM3_bits | GIE)
#define EDU_WAKE_CPU_FROM_ISR() __bic_SR_register_on_exit(CPUOFF)
/* Sleep-mode selection depends on which oscillator a timer uses:
 * With SMCLK Timer_A clocked from the DCO, LPM0 is appropriate;
 * LPM3 normally disables SMCLK, so the timer will stop ticking. */
#endif
