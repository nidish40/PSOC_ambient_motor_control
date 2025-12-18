#include "adc.h"
#include "drivers\clock\clock.h"
#include "psoc4100s_adc_regs.h"


void adc_init(uint8_t pin){
    SAR_CTRL |= ((0x6 << 4) | (0x1 << 7) | (0x7 << 9) | (0x1 << 30)| (0x1 << 31));

    SAR_MUX_SWITCH0 = 0x1<<pin;

    SAR_SAMPLE_CTRL |= (0x0 << 1) | (0x0 << 2) | (0x0 << 16);

    SAR_SAMPLE_TIME01 |= (0xA);
}

void adc_channel_init(uint8_t channel){
    SAR_CHAN_CONFIG(channel) = (0x1 << 0)  |(0x0 << 4) |(0x0 << 9) | (0x0 << 12);
    
    SAR_CHAN_EN |= (1 << channel);
}


void start_adc(void){
    SAR_START_CTRL = (0x1<<0);
}

uint16_t adc_read(uint8_t channel){
    return (SAR_CHAN_RESULT(channel) &0x0FFF);
}

uint8_t check_adc(void){
    if((SAR_INTR&0x01)==0x1){
        return 1;
    }else{
        return 0;
    }
}