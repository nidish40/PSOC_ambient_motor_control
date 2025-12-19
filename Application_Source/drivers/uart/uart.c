#include "uart.h"
#include "drivers/gpio/gpio.h"
#include "drivers/clock/clock.h"

// Register definitions

#define SCB0_CTRL              (*(volatile unsigned int*)0x40240000)
#define SCB0_UART_CTRL         (*(volatile unsigned int*)0x40240040)
#define SCB0_UART_TX_CTRL      (*(volatile unsigned int*)0x40240044)

#define SCB0_TX_FIFO_CTRL      (*(volatile unsigned int*)0x40240204)
#define SCB0_TX_FIFO_STATUS    (*(volatile unsigned int*)0x40240208)
#define SCB0_TX_FIFO_WR        (*(volatile unsigned int*)0x40240240)


#define TX_FIFO_FULL           (1u << 16)

//UART init

void uart_init(void)
{

    gpio_init(7, 1, GPIO_OUTPUT_PUSH_PULL);
    gpio_hsiom_set(7, 1, 9);

    clock_div16_init(5, 155, 0); //route to SCB0

    /* 1. Disable SCB before configuration */
    SCB0_CTRL &= ~(1u << 31);

    /* 2. UART protocol: Standard UART */
    SCB0_UART_CTRL =
        (0u << 24);   // MODE = Standard UART

       // TX Control: Enable + 8-bit data width + LSB first
    SCB0_UART_TX_CTRL = (1 << 0)  // Enable TX (bit 0)
                 | (7u << 1)     // Data width bits (bits 1-4): 7 = 8 bits
                 | (0 << 8);     // LSB first (bit 8 = 0)

    /* 4. Clear TX FIFO */
    SCB0_TX_FIFO_CTRL =
        (0u << 0) |   // Trigger level = 0
        (1u << 16);   // CLEAR FIFO

    /* 5. Select UART mode in SCB_CTRL */
    SCB0_CTRL |=
        (2u << 24)    //Mode = UART 
        | (15u << 0);   // OVS = 16x oversampling

    /* 6. Enable SCB block */
    SCB0_CTRL |= (1u << 31);
}

// TX functions 

void uart_write_char(char c)
{
    while (SCB0_TX_FIFO_STATUS & TX_FIFO_FULL);

    SCB0_TX_FIFO_WR = (unsigned int)c;
}

void uart_write_string(const char *s)
{
    while (*s)
        uart_write_char(*s++);
}
