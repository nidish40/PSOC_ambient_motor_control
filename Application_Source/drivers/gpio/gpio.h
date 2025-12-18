#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "psoc4100s_gpio_regs.h"

//gpio direction 
typedef enum {
    GPIO_INPUT_ANALOG = 0,
    GPIO_INPUT = 1,
    GPIO_INPUT_PULLUP = 2,
    GPIO_INPUT_PULLDOWN = 3,
    GPIO_OUTPUT_PUSH_PULL = 6
} gpio_dir_t;

/*Intitialize a gpio pin
 port : GPIO port number 
 pin: Pin number within the port (0-7)
 dir: output or input*/

 void gpio_init(uint8_t port, uint8_t pin, gpio_dir_t dir);

 /*writes a value to GPIO output pin
 0 -> Low
 1 -> high*/

 void gpio_write(uint8_t port, uint8_t pin, uint8_t value);

 /*reads GPIO input pin returns 0 or 1*/

 uint8_t gpio_read(uint8_t port, uint8_t pin);

 void gpio_hsiom_set(uint8_t port, uint8_t pin, uint8_t function);

 #endif