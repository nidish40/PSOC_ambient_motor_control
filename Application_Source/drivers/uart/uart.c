#include "uart.h"
#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"

/* ================= Register definitions ================= */

#define SCB0_CTRL              (*(volatile unsigned int*)0x40240000)
#define SCB0_UART_CTRL         (*(volatile unsigned int*)0x40240040)
#define SCB0_UART_TX_CTRL      (*(volatile unsigned int*)0x40240044)

#define SCB0_TX_FIFO_CTRL      (*(volatile unsigned int*)0x40240204)
#define SCB0_TX_FIFO_STATUS    (*(volatile unsigned int*)0x40240208)
#define SCB0_TX_FIFO_WR        (*(volatile unsigned int*)0x40240240)

/* TX FIFO full flag */
#define TX_FIFO_FULL           (1u << 16)

/* ================= UART INIT ================= */

void uart_init(void)
{
    gpio_init(1, 1, GPIO_OUTPUT_PUSH_PULL);
    gpio_hsiom_set(1, 1, 9);

    clock_div16_init(5, 13, 0); //route to SCB0

    /* 1. Disable SCB before configuration */
    SCB0_CTRL &= ~(1u << 31);

    /* 2. UART protocol: Standard UART */
    SCB0_UART_CTRL =
        (0u << 24);   // MODE = Standard UART

    /* 3. TX control: 8-bit, LSB first */
    SCB0_UART_TX_CTRL =
        (7u << 0) |   // DATA_WIDTH = 7 → 8 bits
        (0u << 8);    // LSB first

    /* 4. Clear TX FIFO */
    SCB0_TX_FIFO_CTRL =
        (0u << 0) |   // Trigger level = 0
        (1u << 16);   // CLEAR FIFO

    /* 5. Select UART mode in SCB_CTRL */
    SCB0_CTRL =
        (2u << 24);   // MODE = UART

    /* 6. Enable SCB block */
    SCB0_CTRL |= (1u << 31);
}

/* ================= TX FUNCTIONS ================= */

void uart_write_char(char c)
{
    while (SCB0_TX_FIFO_STATUS & TX_FIFO_FULL)
        ;

    SCB0_TX_FIFO_WR = (unsigned int)c;
}

void uart_write_string(const char *s)
{
    while (*s)
        uart_write_char(*s++);
}
