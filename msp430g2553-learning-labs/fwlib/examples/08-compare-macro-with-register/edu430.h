#ifndef EDU430_FULL_H
#define EDU430_FULL_H
/* Select only what you use in 512-byte RAM / 16-kB flash projects.
 * This all-in-one header is convenient for exploration.  Larger labs
 * should include a specific header (e.g. edu430_spi.h) instead. */
#include "edu430_bits.h"
#include "edu430_gpio.h"
#include "edu430_system.h"
#include "edu430_timer.h"
#include "edu430_adc.h"
#include "edu430_uart.h"
#include "edu430_spi.h"
#include "edu430_i2c_soft.h"
#include "edu430_i2c_hw.h"
#include "edu430_scheduler.h"
#include "edu430_pwm.h"
#include "edu430_comparator.h"
#include "edu430_button.h"
/* Board names are separate: pins have electrical identities, not colors. */
#define EDU_BOARD_LED_A BIT0
#define EDU_BOARD_LED_B BIT6
#define EDU_BOARD_BUTTON BIT3
#define EDU_LED_A_ON() EDU_PIN_ON(P1,EDU_BOARD_LED_A)
#define EDU_LED_A_OFF() EDU_PIN_OFF(P1,EDU_BOARD_LED_A)
#define EDU_LED_A_TOGGLE() EDU_PIN_TOGGLE(P1,EDU_BOARD_LED_A)
#define EDU_BUTTON_DOWN() EDU_PIN_LOW(P1,EDU_BOARD_BUTTON)
#endif
