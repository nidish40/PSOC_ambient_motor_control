#ifndef LDR_H
#define LDR_H

#include <stdint.h>

/* Initialize LDR ADC */
void ldr_init(void);

/* Get ambient light as percentage
 * 0   = very dark
 * 100 = very bright
 */
uint8_t ldr_get_light_percent(void);

#endif
