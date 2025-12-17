#include "drivers/timers/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"
#include <stdbool.h>


volatile uint8_t button_pressed = 0;
volatile uint8_t timer_flag = 0;

#define BTN_PRT 3
#define BTN_PIN 7



int main(void)
{
    /*clock init*/
    clock_div16_init(0, 24, 8); //set clk_peri div16_0 for 24 Mhz divide by 24 and route to tcpwm2 clk (peripheral 8)

    /*timer init*/
    //*((uint32_t *)0xE000E410) = (1 << 30); //Timer 2 IRQn=19 Priority  1 set
    NVIC_EnableIRQ(19); //enable Timer 2 IRQn=19
    NVIC_SetPriority(19, 0); //set Timer 2 IRQn=19 Priority 
    NVIC_ClearPendingIRQ(19); //clear any pending interrupt for Timer 2 in NVIC
	
    /*TIMER 2 - INIT*/
    *((uint32_t *)0x40200000) &=~ (1<< 2); //Disable Timer 2  in TCPWM_CTRL Register

    *((uint32_t *)0x40200188) = 0; //Clear the counter register of  TCPWM2 TCPWM_CNT2_COUNTER Register

    *((uint32_t *)0x402001A8) |= (0<<2); //Clear the  register of  TCPWM2 TCPWM_CNT2_TR_CTRL2 Register

	*((uint32_t *)0x40200194)  = (50000-1); //Set the Period Register of TCPWM2 TCPWM_CNT2_PERIOD Register

	*((uint32_t *)0x40200180) |= (0 << 24); //Mode configuration of for TCPWM2, TCPWM_CNT2_CTRL Regsiter 

	*((uint32_t *)0x402001B8) |= (1 << 0); // Set  interrupt mask  by enabling the Interrupt in TCPWM_CNT2_INTR_MASK Register for TCPWM2

	*((uint32_t *)0x402001B0) = (1<<0); //Clear any previoius interrupt in  interrupt register in TCPWM_CNT2_INTR Register of TCPWM2 

    *((uint32_t *)0x40200000) |= (1<< 2); //Enable Timer 2  in TCPWM_CTRL Register

    *((uint32_t *)0x40200008) |= (1 << 26); //Triger start Timer 2  in TCPWM_CTRL Register

    /*led init*/
    gpio_init(2,2, GPIO_OUTPUT_PUSH_PULL); //init p2.2 as output

    gpio_init(2,0, GPIO_OUTPUT_PUSH_PULL); //init p2.0 as output

    /*button*/
    gpio_init(3,7, GPIO_INPUT_PULLUP); //init p3.7 as input with pullup

    GPIO_PORT(3)->INTR_CFG |= (0x2 << (BTN_PIN * 2)); //set interrupt on falling edge for p3.7
    GPIO_PORT(3)->INTR |= (1 << BTN_PIN); //clear any pending interrupt for p3.7


    /*configuring interrupt*/
    

    NVIC_SetPriority(3, 1); //set highest priority for button
    NVIC_EnableIRQ(3); //enable GPIO P3 interrupt in NVIC
    
    
    NVIC_ClearPendingIRQ(3); //clear any pending interrupt for GPIO P3 in NVIC

    enable_irq(); //enable global interrupts

    while (1)
    {   

        
    }
}

void ioss_interrupts_gpio_3_IRQHandler(void)
{
    //check if button is pressed
    GPIO_PORT(3)->INTR |= (1 << BTN_PIN); //clear interrupt
    
    if (gpio_read(3,7) == 0) //active low
    {
        button_pressed ^= 1; //toggle button pressed state
        gpio_write(2,0, !button_pressed); //toggle led at p2.0
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
