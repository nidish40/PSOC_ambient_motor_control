#ifndef PSOC4100S_GPIO_REGS_H
#define PSOC4100S_GPIO_REGS_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t DR;        /* 0x00 Data Register */
    volatile uint32_t PS;        /* 0x04 Pin Status */
    volatile uint32_t PC;        /* 0x08 Port Configuration */
    volatile uint32_t INTR_CFG;  /* 0x0C Interrupt Config */
    volatile uint32_t INTR;      /* 0x10 Interrupt Status */
    uint32_t RESERVED0[1];
    volatile uint32_t PC2;       /* 0x18 Port Config 2 */
    uint32_t RESERVED1[9];
    volatile uint32_t DR_SET;    /* 0x40 Data Set */
    volatile uint32_t DR_CLR;    /* 0x44 Data Clear */
    volatile uint32_t DR_INV;    /* 0x48 Data Invert */
} GPIO_PRT_Type;


/*GPIO base = 0x40040000
port offset = 0x100u*/

#define GPIO_BASE_ADDR       0x40040000ul
#define GPIO_PORT_OFFSET     0x100ul

#define GPIO_PORT(port) ((GPIO_PRT_Type *)(GPIO_BASE_ADDR + (GPIO_PORT_OFFSET * (port))))

/*HSIOM base = 0x40020000
offset = 0x100*/

#define HSIOM_BASE_ADDR     0x40020000ul
#define HSIOM_PORT_OFFSET   0x100ul

#define HSIOM_PORT(port) (*(volatile uint32_t *)(HSIOM_BASE_ADDR + (HSIOM_PORT_OFFSET * (port))))

#define HSIOM_SEL_GPIO 0x00ul

/* ---------------- DRIVE MODE VALUES ---------------- */
#define GPIO_DM_ALG_HIZ     (0x0u)
#define GPIO_DM_HIGHZ       (0x1u)
#define GPIO_DM_RES_UP   (0x02u)
#define GPIO_DM_RES_DWN  (0x03u)
#define GPIO_DM_STRONG      (0x6u)

#endif /* PSOC4100S_GPIO_REGS_H */