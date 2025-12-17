#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

/*
 * Configure a 16-bit peripheral clock divider
 *
 * divider_num : divider index (0, 1, 2, ...)
 * divider_val : division factor (e.g. 12 → clk/12)
 * peripheral : peripheral selection, tim0, tim1 ...
 * 
 * 
 */

#define CLK_PERI_CMD *(volatile uint32_t*) (0x40010000ul)

#define PERI_PCLK_CTL(peripheral) (*(volatile uint32_t *)(0x40010100ul + ((uint32_t)(peripheral) * 4u)))

#define PERI_DIV_16_CTL(divider) (*(volatile uint32_t *)(0x40010300ul + ((uint32_t)(divider) * 4u)))

void clock_div16_init(uint8_t divider_num, uint16_t divider_val, uint8_t peripheral);

#endif
