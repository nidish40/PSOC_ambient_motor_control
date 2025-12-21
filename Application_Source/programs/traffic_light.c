#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"
#include "drivers/uart/uart.h"
#include "drivers/timers/timer.h"
#include "drivers/adc/adc.h"
#include "drivers/timers/psoc4100s_tcpwm_regs.h"
#include "./../Special_Libraries/cmsis_gcc.h"

volatile uint16_t adc_res = 0;
volatile uint8_t adc_ready = 0;

/* Timer period in milliseconds */
#define TIMER_PERIOD_MSEC   10U

/*state variables*/
typedef enum {
    LEVEL_0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4 
} light_level_t;

volatile light_level_t curr_level = LEVEL_0;

const uint8_t led_count[] = {0,2,4,6,9};

/*gpio build leds*/
static const struct {
    uint8_t port;
    uint8_t pin;
} leds[] = { {2,2}, {2,0}, {1,6}, {1,4}, {1,2}, {1,0},
            {5,2}, {5,5}, {5,7}};
 
    
/*helper functions*/
void update_state(uint16_t adc);
void update_leds(light_level_t level);


int main()
{  
    /*----init gpio----*/
    for(uint8_t i =0;i<9;i++)
        gpio_init(leds[i].port, leds[i].pin, GPIO_OUTPUT_PUSH_PULL);

    
    /*user led used for interrupt debugging*/
    gpio_init(3,4,6);

    /*----ldr init-----*/
    clock_div16_init(0,2,18);

    adc_init(1);

    SAR_SAMPLE_CTRL |=  (2<<4) | (1<<7); //set average count to 2 (8 samples per adc results) -> 4th bit
    //, set AVG_SHIFTING -> 7th bit

    adc_channel_init(0,1);
    SAR_CHAN_CONFIG(0) |= (1<<10); //set averaging for channel 0
    *((uint32_t*)0x403A0218) = 1<<0; //set EOS interrupt in SAR_INTR_MASK register

   
    /*----TIMER 2 - config---*/
    clock_div16_init(3,24000,8); //TCPWM2 Peripheral clock init, feeds 1kHz clock to timer 2

    timer_init(2, TIMER_PERIOD_MSEC);

    timer_start(2);

    /*---INTERRUPT----*/

    /*Timer 2 interrupt*/
    NVIC_SetPriority(19, 1);
    NVIC_ClearPendingIRQ(19);
    NVIC_EnableIRQ(19);

    /*SAR interrupt*/
    NVIC_SetPriority(25, 0);
    NVIC_ClearPendingIRQ(25);
    NVIC_EnableIRQ(25);

    /* Enable Interrupts at CPU level */
    enable_irq();

    while (1)
    {
        if(adc_ready){
            adc_ready = 0;

            update_state(adc_res);
            update_leds(curr_level);
        }  
    }

    return 0;
}


/*Timer 2 interrupt*/
void tcpwm_interrupts_2_IRQHandler(void)
{
    //check if timer expired
    if (*((uint32_t *)0x402001B0) & (1 << 0)) //check TC bit in TCPWM2 TCPWM_CNT2_INTR Register
    {
        //clear interrupt
        *((uint32_t *)0x402001B0) = (1 << 0); //clear TC bit in TCPWM2 TCPWM_CNT2_INTR Register
        
        start_adc();
    }
}

void pass_0_interrupt_sar_IRQHandler(void){
    if((SAR_INTR&0x01)==0x1){ //wait for EOS interrupt
        SAR_INTR |= 0x01; //clear EOS interrupt
        
        adc_res = adc_read(0); //read adc result
        adc_ready = 1;
    }
}

void update_state(uint16_t adc){
    switch (curr_level){
        case LEVEL_0:
            if(adc > 1000) curr_level = LEVEL_1;
            break;
        case LEVEL_1:
            if(adc > 1600) curr_level = LEVEL_2;
            else if(adc < 900) curr_level = LEVEL_0;
            break;
        case LEVEL_2:
            if(adc > 2100) curr_level = LEVEL_3;
            else if(adc < 1400) curr_level = LEVEL_1;
            break;
        case LEVEL_3:
            if(adc > 2600) curr_level = LEVEL_4;
            else if (adc < 1900) curr_level = LEVEL_2;
            break;
        case LEVEL_4:
            if(adc < 2400) curr_level = LEVEL_3;
            break;
    }
}

void update_leds(light_level_t level){
    uint8_t on = led_count[level];

    for(uint8_t i =0;i<9;i++)
        gpio_write(leds[i].port, leds[i].pin, !(i<on));
}


