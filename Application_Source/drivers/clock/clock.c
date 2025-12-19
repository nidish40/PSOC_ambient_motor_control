#include "clock.h"

void clock_div16_init(uint8_t divider_num, uint16_t divider_val, uint8_t peripheral)
{
    /* Disable the divider before configuration */
    CLK_PERI_CMD = (1<<30)|(1<<6)|(divider_num<<0); //disable ->30, divider type (integer or fractional) ->6 , divider number ->0

    /* Set the division value */
    PERI_DIV_16_CTL(divider_num) = ((uint32_t)divider_val-1ul)<<8; //divider value (clock/divider value)

    /* Enable the divider */
    CLK_PERI_CMD = (1<<31) |(3<<14) |(63<<8) |(1<<6)|(divider_num<<0); //enable ->31, default values ->14 and 6, divider type -> 6, divider number ->0

    /*route to peripharal*/
    PERI_PCLK_CTL(peripheral) = (1<<6)|(divider_num<<0); //divider type -> 6, divide block number ->0

}