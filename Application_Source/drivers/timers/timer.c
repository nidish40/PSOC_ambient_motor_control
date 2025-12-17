#include "drivers/timers/timer.h"
#include "psoc4100s_tcpwm_regs.h"
#include <stdio.h>

typedef struct
{
    uint32_t step;
    uint32_t tcpwm_ctrl;
    uint32_t counter;
    uint32_t tr_ctrl2;
    uint32_t period;
    uint32_t cnt_ctrl;
    uint32_t intr_mask;
    uint32_t intr;

} timer_debug_t;

volatile timer_debug_t timer_dbg;

void timer_init(uint8_t cnt, uint32_t period)
{
    TCPWM_CTRL &= ~(1<<cnt); //disable the timer

    TCPWM_CNT_REG(cnt, TCPWM_COUNTER_OFFSET) = 0; //clear the counter reg

    TCPWM_CNT_REG(cnt, TCPWM_TR_CTRL2_OFFSET) = 0; // when OF happens, set line_out to 1

    TCPWM_CNT_REG(cnt, TCPWM_PERIOD_BUFF_OFFSET) = period - 1; //set the period

    TCPWM_CNT_REG(cnt, TCPWM_CNT_CTRL_OFFSET) |= 0<<24; //set mode to timer

    TCPWM_CNT_REG(cnt, TCPWM_INTR_MASK_OFFSET) |= 1<<0; //enable terminal count interrupt

    TCPWM_CNT_REG(cnt, TCPWM_INTR_OFFSET) |= 1<<0; //clear any pending interrupts
}

void timer_start(uint8_t cnt)
{
    TCPWM_CTRL |= (1<<cnt); //enable the timer

    TCPWM_CMD |= (1<<(24 + cnt)); //start the timer
}

uint8_t timer_expired(uint8_t cnt)
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