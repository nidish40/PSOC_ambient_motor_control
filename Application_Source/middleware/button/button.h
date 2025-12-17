#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

void button_init(uint8_t port, uint8_t pin);

/* Returns 1 if button is pressed (debounced), else 0 */
uint8_t button_is_pressed(void);

#endif
