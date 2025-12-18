#include "button.h"
#include "drivers/gpio/gpio.h"

/* ================= CONFIG ================= */

#define BUTTON_PORT   3
#define BUTTON_PIN    7

/* ================= STATE ================= */

static volatile uint8_t button_pressed_flag = 0;

/* ================= API ================= */

void button_init(void)
{
    gpio_init(BUTTON_PORT, BUTTON_PIN, GPIO_INPUT_PULLUP);
    GPIO_PORT(3)->INTR_CFG |= (0x2 << (BUTTON_PIN * 2)); //set interrupt on falling edge for p3.7
    GPIO_PORT(3)->INTR |= (1 << BUTTON_PIN); //clear any pending interrupt for p3.7
    button_pressed_flag = 0;
}

void button_isr_notify(void)
{
    /* ISR-safe: just set a flag */
    button_pressed_flag = 1;
}

uint8_t button_was_pressed(void)
{
    if (button_pressed_flag)
    {
        button_pressed_flag = 0;  // clear after read
        return 1;
    }
    return 0;
}
