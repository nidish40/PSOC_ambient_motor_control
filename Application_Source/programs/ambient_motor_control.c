#include "middleware/ldr/ldr.h"
#include "middleware/motor/motor.h"
#include "middleware/button/button.h"
#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"
#include "drivers/uart/uart.h"
#include "drivers/timers/timer.h"
#include "drivers/timers/psoc4100s_tcpwm_regs.h"
#include "./../Special_Libraries/cmsis_gcc.h"


int main(void)
{
    clock_div16_init(0, 2, 18); //imo clock input to ldr
    gpio_init(2,1, GPIO_INPUT_ANALOG); //ldr input in p2.1
    ldr_init(); //initialize ldr to start reading

    motor_init(); //initilize motor
    button_init(); //on board button used to toggle direction

    NVIC_ClearPendingIRQ(3); //clear any pending interrupt for GPIO port 3 in NVIC
    NVIC_EnableIRQ(3); //enable GPIO port 3 interrupt in NVIC
    NVIC_SetPriority(3, 0); //set GPIO port 3 interrupt priority to 2
    NVIC_ClearPendingIRQ(3); //clear any pending interrupt for GPIO port 3 in NVIC

    enable_irq(); //enable global interrupts

    uart_init(); //initialize uart for debugging

    while (1)
    {
        uart_write_char('a'); 
        static uint32_t ldr_div = 0;

        ldr_div++;
        if (ldr_div >= 500)   //wait interval before reading
        {
            
            ldr_div = 0;

            uint8_t light = ldr_get_light_percent();
            motor_set_target_speed(100-light);
        }

        motor_update();
    }
    return 0;

}

void ioss_interrupts_gpio_3_IRQHandler(void)
{
    //check if button is pressed

    //check if timer expired
    if (*((uint32_t *)0x402001B0) & (1 << 0)) //check TC bit in TCPWM2 TCPWM_CNT2_INTR Register
    {
        //clear interrupt
        *((uint32_t *)0x402001B0) = (1 << 0); //clear TC bit in TCPWM2 TCPWM_CNT2_INTR Register
        timer_flag ^= 1; //toggle timer flag
        gpio_write(2,2, timer_flag); //set p2.2 high

    }
}