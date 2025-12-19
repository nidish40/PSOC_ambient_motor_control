#ifndef PSOC4100S_TCPWM_REGS_H
#define PSOC4100S_TCPWM_REGS_H

#include <stdint.h>

/* Base address of TCPWM CNT0 */
#define TCPWM_CNT_BASE       0x40200100UL
#define TCPWM_CNT_OFFSET     0x40UL

/* Base address macro */
#define TCPWM_CNT_BASE_ADDR(n) (TCPWM_CNT_BASE + ((uint32_t)(n) * TCPWM_CNT_OFFSET))

//address of TCPWM ctrl block
#define TCPWM_CTRL (*(volatile uint32_t*)(0x40200000UL))

//address of TCPWM cmd block
#define TCPWM_CMD (*(volatile uint32_t*)(0x40200008UL))

/* Register offsets (from TRM) */
#define TCPWM_CNT_CTRL_OFFSET        0x00 //control mode of block
#define TCPWM_STATUS_OFFSET          0x04
#define TCPWM_COUNTER_OFFSET         0x08
#define TCPWM_CC_OFFSET              0x0C
#define TCPWM_CC_BUFF_OFFSET         0x10
#define TCPWM_PERIOD_OFFSET          0x14
#define TCPWM_PERIOD_BUFF_OFFSET     0x18
#define TCPWM_TR_CTRL0_OFFSET        0x20
#define TCPWM_TR_CTRL1_OFFSET        0x24
#define TCPWM_TR_CTRL2_OFFSET        0x28
#define TCPWM_INTR_OFFSET            0x30
#define TCPWM_INTR_SET_OFFSET        0x34
#define TCPWM_INTR_MASK_OFFSET       0x38
#define TCPWM_INTR_MASKED_OFFSET     0x3C


/* Register access macros for TCPWM block */
#define TCPWM_CNT_REG(cnt, offset) (*(volatile uint32_t *)(TCPWM_CNT_BASE_ADDR(cnt) + (offset)))

/* Bits */
#define TCPWM_CTRL_ENABLE_CNT(cnt)   (1u << (cnt))
#define TCPWM_CTRL_START_CNT(cnt)    (1u << (24 + (cnt)))

#define TCPWM_INTR_TC                (1u << 0)

/* Mode field */
#define TCPWM_MODE_TIMER             (0u << 24)

#endif