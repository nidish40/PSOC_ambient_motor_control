#include "drivers/timers/timer.h"
#include "psoc4100s_tcpwm_regs.h"
#include <stdio.h>

void timer_init(uint8_t cnt, uint32_t period)
{
    TCPWM_CTRL &= ~(1<<cnt); //disable the timer

    TCPWM_CNT_REG(cnt, TCPWM_COUNTER_OFFSET) = 0; //clear the counter reg

    TCPWM_CNT_REG(cnt, TCPWM_TR_CTRL2_OFFSET) = 0; // when OF happens, set line_out to 1

    TCPWM_CNT_REG(cnt, TCPWM_PERIOD_OFFSET) = period - 1; //set the period

    TCPWM_CNT_REG(cnt, TCPWM_CNT_CTRL_OFFSET) |= 0<<24; //set mode of TCPWM blcok to timer

    TCPWM_CNT_REG(cnt, TCPWM_INTR_MASK_OFFSET) |= 1<<0; //enable terminal count interrupt

    TCPWM_CNT_REG(cnt, TCPWM_INTR_OFFSET) |= 1<<0; //clear any pending interrupts
}

void timer_start(uint8_t cnt)
{
    TCPWM_CTRL |= (1<<cnt); //enable the timer

    TCPWM_CMD |= (1<<(24 + cnt)); //start the timer
}

uint8_t timer_expired(uint8_t cnt) //checks if timer is done
{
    if (TCPWM_CNT_REG(cnt, TCPWM_INTR_MASKED_OFFSET) & (1<<0)) //if TC is set then return 1
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void timer_clear(uint8_t cnt) 
{
    TCPWM_CNT_REG(cnt, TCPWM_INTR_OFFSET) |= 1<<0; //clear the TC interrupt
}

void pwm_init(uint8_t cnt, uint32_t period, uint32_t compare)
{
    TCPWM_CTRL &= ~(1<<cnt); //disable the timer

    TCPWM_CNT_REG(cnt, TCPWM_COUNTER_OFFSET) = 0; //clear the counter reg

    TCPWM_CNT_REG(cnt, TCPWM_TR_CTRL2_OFFSET) = (3<<4) | (0<<2) | (1<<0); //OF: set line_out to 1, CC match: clear line_out to 0

    TCPWM_CNT_REG(cnt, TCPWM_CC_OFFSET) = (compare - 1); //set compare value

    TCPWM_CNT_REG(cnt, TCPWM_PERIOD_OFFSET) = (period - 1); //set the period

    TCPWM_CNT_REG(cnt, TCPWM_CNT_CTRL_OFFSET) |= (4<<24) | (1<<3); //PWM mode set, continous mode by default bit 18, PWM stop on kill 
}

void pwm_start(uint8_t cnt)
{
    TCPWM_CTRL |= (1<<cnt); //enable the timer

    TCPWM_CMD |= (1<<(8 + cnt)); //reload the timer
}