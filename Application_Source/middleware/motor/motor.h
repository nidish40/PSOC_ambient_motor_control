#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/* Motor direction */
typedef enum {
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_REVERSE
} motor_dir_t;

/* Initialize motor middleware */
void motor_init(void);

/* Set desired speed (0–100 %) */
void motor_set_target_speed(uint8_t percent);

/* Set motor direction */
void motor_set_direction(motor_dir_t dir);

/* Toggle motor direction */
void motor_toggle_direction(void);

/* Periodic update function (call every 5–10 ms) */
void motor_update(void);

#endif
