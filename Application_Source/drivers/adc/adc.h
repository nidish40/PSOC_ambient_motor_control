#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "psoc4100s_adc_regs.h"

/* Initialize ADC core (clock, SAR, reference) */
void adc_init(uint8_t port);

/* Configure and enable an ADC channel */
void adc_channel_init(uint8_t channel, uint8_t pin);

/* Perform a software-triggered conversion */
uint16_t adc_read(uint8_t channel);

/*start adc conversion*/
void start_adc(void);

uint8_t check_adc();

#endif
