#include "uart.h"
#include "drivers/clock/clock.h"

/* ================= Register definitions ================= */

#define SCB0_CTRL        (*(volatile uint32_t*)0x40240000)
#define SCB0_UART_CTRL   (*(volatile uint32_t*)0x40240040)
#define SCB0_TX_CTRL    (*(volatile uint32_t*)0x40080048)
#define SCB0_TX_FIFO_CTRL (*(volatile uint32_t*)0x40080080)
#define SCB0_TX_FIFO_STATUS (*(volatile uint32_t*)0x40080084)
#define SCB0_TX_FIFO_WR (*(volatile uint32_t*)0x40080088)

#define HSIOM_PORT0_PIN4 (*(volatile uint32_t*)0x40040108)

/* ================= Config ================= */

#define UART_TX_PIN_HSIOM 9   // SCB UART TX
#define UART_FIFO_FULL   (1 << 16)

/* ================= API ================= */

void uart_init(void)
{
    /* Route clock to SCB0 (use existing clock API) */
    clock_div16_init(4, 13, 5);  // example: 24MHz/13 ≈ 1.84MHz → good for 115200

    /* Disable SCB */
    SCB0_CTRL &= ~1;

    /* UART mode */
    SCB0_CTRL = (2 << 24);

    /* UART control: enable TX */
    SCB0_UART_CTRL = (1 << 0);

    /* TX control */
    SCB0_TX_CTRL = (1 << 0);

    /* FIFO */
    SCB0_TX_FIFO_CTRL = 0;

    /* Route TX pin */
    HSIOM_PORT0_PIN4 = UART_TX_PIN_HSIOM;

    /* Enable SCB */
    SCB0_CTRL |= 1;
}

void uart_write_char(char c)
{
    while (SCB0_TX_FIFO_STATUS & UART_FIFO_FULL);
    SCB0_TX_FIFO_WR = c;
}

void uart_write_string(const char *str)
{
    while (*str)
        uart_write_char(*str++);
}

void uart_write_uint(uint32_t val)
{
    char buf[10];
    int i = 0;

    if (val == 0)
    {
        uart_write_char('0');
        return;
    }

    while (val > 0)
    {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }

    while (i--)
        uart_write_char(buf[i]);
}
