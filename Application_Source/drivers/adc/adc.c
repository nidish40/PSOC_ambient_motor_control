#include "adc.h"
#include "drivers\clock\clock.h"
#include "psoc4100s_adc_regs.h"


void adc_init(uint8_t pin){
    /*4->VREF select, 7->bypass capacitor, 9->NEgative select, 30->switch_disable, 31-> disable*/
    SAR_CTRL |= ((0x6 << 4) | (0x1 << 7) | (0x7 << 9) | (0x1 << 30)| (0x1 << 31));

    /*close switch between pin and vplus*/
    SAR_MUX_SWITCH0 = 0x1<<pin;

    /*1->result alignment, 2->signed/unsigned, 16->continous scan*/
    SAR_SAMPLE_CTRL |= (0x0 << 1) | (0x0 << 2) | (0x0 << 16); 

    /*sample time for a singal*/
    SAR_SAMPLE_TIME01 |= (0xA);
}

void adc_channel_init(uint8_t channel, uint8_t pin){
    /*0->  4->SARMUC pins, 4->PORT address, 9->Resolution of res, 12->which time sample for sampling clock*/
    /*HERE, 1st bit*/SAR_CHAN_CONFIG(channel) = (pin << 0)  |(0x0 << 4) |(0x0 << 9) | (0x0 << 12);
    //CHECK HERE

    /*enable channel routing*/
    SAR_CHAN_EN |= (1 << channel);
}


void start_adc(void){
    /*0-> start SAR*/
    SAR_START_CTRL = (0x1<<0);
}

uint16_t adc_read(uint8_t channel){
    return (SAR_CHAN_RESULT(channel) &0x0FFF);
}

uint8_t check_adc(void){
    if((SAR_INTR&0x01)==0x1){ //wait for EOS interrupt
        return 1;
    }else{
        return 0;
    }
}