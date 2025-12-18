#include "gpio.h"
#include "psoc4100s_gpio_regs.h"

/* Each pin uses 3 bits in the PC register */
#define GPIO_PC_BITS_PER_PIN   (3u)

/* Each pin uses 4 bits in the HSIOM register */
#define HSIOM_BITS_PER_PIN     (4u)

void gpio_hsiom_set(uint8_t port, uint8_t pin, uint8_t function)
{
    //hsiom configuring for gpio
    uint32_t hsiom_mask = (uint32_t)(0xFu << (pin * HSIOM_BITS_PER_PIN));
    uint32_t hsiom_val = (uint32_t)(function << (pin * HSIOM_BITS_PER_PIN));

    HSIOM_PORT(port) &= ~hsiom_mask; //clear the hsiom mux for that pin
    HSIOM_PORT(port) |= hsiom_val; //set the hsiom bits for gpio
}

void gpio_init(uint8_t port, uint8_t pin, gpio_dir_t dir){
    
    //configure drive mode in pc
    uint32_t pc_mask = (uint32_t)(0x7u<< (pin* GPIO_PC_BITS_PER_PIN));
    GPIO_PORT(port)->PC &= ~pc_mask;

    //setting drive mode
    if(dir==GPIO_OUTPUT_PUSH_PULL){
        GPIO_PORT(port)->PC |= (GPIO_DM_STRONG << (pin*GPIO_PC_BITS_PER_PIN));

        GPIO_PORT(port)->DR_SET = (1u<<pin);
    }
    else if(dir==GPIO_INPUT){
        GPIO_PORT(port)->PC |= (GPIO_DM_HIGHZ << (pin*GPIO_PC_BITS_PER_PIN));
    }
    else if(dir==GPIO_INPUT_ANALOG){
        GPIO_PORT(port)->PC |= (GPIO_DM_ALG_HIZ << (pin*GPIO_PC_BITS_PER_PIN));
    }
    else if(dir==GPIO_INPUT_PULLUP){
        GPIO_PORT(port)->PC |= (GPIO_DM_RES_UP << (pin*GPIO_PC_BITS_PER_PIN));

        /* IMPORTANT: enable pull-up by writing 1 to DR */
        GPIO_PORT(port)->DR_SET = (1u<<pin);
    }
    else if(dir==GPIO_INPUT_PULLDOWN){
        GPIO_PORT(port)->PC |= (GPIO_DM_RES_DWN << (pin*GPIO_PC_BITS_PER_PIN));

        /* Enable pull-down by writing 0 to DR */
        GPIO_PORT(port)->DR_CLR = (1u << pin);
    }
}

void gpio_write(uint8_t port, uint8_t pin, uint8_t value)
{
    if (value)
    {
        GPIO_PORT(port)->DR_SET = (1u << pin);
    }
    else
    {
        GPIO_PORT(port)->DR_CLR = (1u << pin);
    }
}

uint8_t gpio_read(uint8_t port, uint8_t pin){
    return (uint8_t)((GPIO_PORT(port)->PS >> pin) & 0x01u);
}


