#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/* Initialize button */
void button_init(void);

/* Called from GPIO ISR */
void button_isr_notify(void);

/* Check if button was pressed */
uint8_t button_was_pressed(void);

#endif
