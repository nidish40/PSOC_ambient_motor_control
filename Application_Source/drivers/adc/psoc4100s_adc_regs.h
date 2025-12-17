#ifndef PSOC4100S_ADC_REGS_H
#define PSOC4100S_ADC_REGS_H

#include <stdint.h>

#define SAR_CTRL (*(volatile uint32_t*) 0x403A0000)

#define SAR_MUX_SWITCH0 (*(volatile uint32_t*) 0x403A0300)

#define SAR_SAMPLE_CTRL (*(volatile uint32_t*) 0x403A0004)

#define SAR_SAMPLE_TIME01 (*(volatile uint32_t*) 0x403A0010)

#define SAR_SAMPLE_TIME23 (*(volatile uint32_t*) 0x403A0014)

#define SAR_CHAN_CONFIG(channel) (*(volatile uint32_t*) (0x403A0080ul + ((uint32_t)(channel) * 4u)))

#define SAR_CHAN_EN (*(volatile uint32_t*) 0x403A0020)

#define SAR_START_CTRL (*(volatile uint32_t*) 0x403A0024)

#define SAR_INTR (*(volatile uint32_t*) 0x403A0210)

#define SAR_CHAN_RESULT(channel) (*(volatile uint32_t*) (0x403A0180ul + ((uint32_t)(channel) * 4u)))

#endif