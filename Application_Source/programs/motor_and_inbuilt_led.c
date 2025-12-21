#include "drivers/timers/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"
#include "drivers/timers/psoc4100s_tcpwm_regs.h"
#include <stdbool.h>


volatile uint8_t button_pressed = 0;
volatile uint8_t timer_flag = 0;
volatile uint16_t timer_count = 0;

#define BTN_PRT 3
#define BTN_PIN 7



int main(void)
{
    /*clock init*/
    clock_div16_init(0, 24, 8); //set clk_peri div16_0 for 24 Mhz divide by 24 and route to tcpwm2 clk (peripheral 8)

    /*timer init*/
    //*((uint32_t *)0xE000E410) = (1 << 30); //Timer 2 IRQn=19 Priority  1 set
    NVIC_EnableIRQ(19); //enable Timer 2 IRQn=19
    NVIC_ClearPendingIRQ(19); //clear any pending interrupt for Timer 2 in NVIC
    NVIC_SetPriority(19, 0); //set Timer 2 IRQn=19 Priority 
	
    /*TIMER 2 - INIT*/

    timer_init(2, 50000); //initialize timer 2 with period of 50000 ticks (24MHz/50000 = 480 Hz)

    timer_start(2); //start timer 2

    /*led init*/
    gpio_init(2,2, GPIO_OUTPUT_PUSH_PULL); //init p2.2 as output

    gpio_init(2,0, GPIO_OUTPUT_PUSH_PULL); //init p2.0 as output

    /*pwm led*/
    gpio_init(1, 6, GPIO_OUTPUT_PUSH_PULL); //init p1.6 as output
    gpio_hsiom_set(1,6, 8); //set hsiom to tcpwm7, act_0 of hsiom port 1 is tcpwm7 lineout 

    clock_div16_init(1, 24, 13); //24Mhz/24 = 1MHz clock to tcpwm7 (peripheral 13)
    pwm_init(7, 10000, 9000); //initialize pwm on tcpwm7 with period of 10000 ticks (1MHz/10000 = 100 Hz) and 20% duty cycle
    pwm_start(7);
    

    /*button*/
    gpio_init(3,7, GPIO_INPUT_PULLUP); //init p3.7 as input with pullup

    GPIO_PORT(3)->INTR_CFG |= (0x2 << (BTN_PIN * 2)); //set interrupt on falling edge for p3.7
    GPIO_PORT(3)->INTR |= (1 << BTN_PIN); //clear any pending interrupt for p3.7

    /*motor*/
    gpio_init(5,3, GPIO_OUTPUT_PUSH_PULL); //init p5.3 as output
    gpio_write(5,3, 0); //set p5.3 high to enable motor driver

    gpio_init(5,5, GPIO_OUTPUT_PUSH_PULL); //init p5.5 as output
    gpio_write(5,5, 1); //set p5.5 high to set motor direction

    gpio_init(1,2, GPIO_OUTPUT_PUSH_PULL); //init p1.2 as output
    gpio_write(1,2, 1); //set p1.2 to high to enable motor pwm input


    /*configuring interrupt*/
    
    NVIC_SetPriority(3, 1); //set second highest priority for button
    NVIC_EnableIRQ(3); //enable GPIO P3 interrupt in NVIC
    
    
    NVIC_ClearPendingIRQ(3); //clear any pending interrupt for GPIO P3 in NVIC

    enable_irq(); //enable global interrupts

    while (1)
    {   
        for(volatile uint16_t i=0; i<10000;i=i+1000){   
            TCPWM_CNT_REG(7, TCPWM_CC_OFFSET) = i-1; //initialize pwm on tcpwm7 with period of 48000 ticks (24MHz/48000 = 500 Hz) and 50% duty cycle
            for(volatile uint32_t i=0;i<5000000;i++);
        }

       //timer_count = TCPWM_CNT_REG(7, TCPWM_COUNTER_OFFSET); //read current timer count
        
    }
}

void ioss_interrupts_gpio_3_IRQHandler(void)
{
    //check if button is pressed
    GPIO_PORT(3)->INTR |= (1 << BTN_PIN); //clear interrupt
    
    if (gpio_read(3,7) == 0) //active low
    {
        button_pressed ^= 1; //toggle button pressed state
        gpio_write(5,3, !button_pressed);
        gpio_write(5,5, button_pressed); //turn on at p1.2
    }
}

void tcpwm_interrupts_2_IRQHandler(void)
{
    //check if timer expired
    if (*((uint32_t *)0x402001B0) & (1 << 0)) //check TC bit in TCPWM2 TCPWM_CNT2_INTR Register
    {
        //clear interrupt
        *((uint32_t *)0x402001B0) = (1 << 0); //clear TC bit in TCPWM2 TCPWM_CNT2_INTR Register
        timer_flag ^= 1; //toggle timer flag
        gpio_write(2,2, timer_flag); //set p2.2 high

    }
}
