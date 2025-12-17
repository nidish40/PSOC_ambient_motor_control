#include "button.h"
#include "drivers/gpio/gpio.h"

#define DEBOUNCE_DELAY 5000

static uint8_t btn_port;
static uint8_t btn_pin;

static void delay(void)
{
    for (volatile uint32_t i = 0; i < DEBOUNCE_DELAY; i++);
}

void button_init(uint8_t port, uint8_t pin)
{
    btn_port = port;
    btn_pin  = pin;

    gpio_init(btn_port, btn_pin, GPIO_INPUT_PULLUP);
}

uint8_t button_is_pressed(void)
{
    uint8_t first = gpio_read(btn_port, btn_pin);
    delay();
    uint8_t second = gpio_read(btn_port, btn_pin);

    /* Active LOW button */
    if ((first == 0) && (second == 0))
        return 1;

    return 0;
}
