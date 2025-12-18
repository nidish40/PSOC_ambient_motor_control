#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * Initialize TCPWM timer
 * cnt     : counter number (0–7)
 * period  : timer period (ticks)
 */
void timer_init(uint8_t cnt, uint32_t period);

/* Start timer */
void timer_start(uint8_t cnt);

/* Check terminal count */
uint8_t timer_expired(uint8_t cnt);

/* Clear terminal count */
void timer_clear(uint8_t cnt);

void pwm_init(uint8_t cnt, uint32_t period, uint32_t compare);

void pwm_start(uint8_t cnt);

#endif
