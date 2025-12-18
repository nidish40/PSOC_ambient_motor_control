#include "motor.h"
#include "drivers/timers/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/timers/psoc4100s_tcpwm_regs.h"

///User config

#define MOTOR_PWM_CNT        7       // TCPWM counter number
#define MOTOR_PWM_PERIOD     1000

#define MOTOR_DIR_PORT       1
#define MOTOR_DIR_PIN        6

#define MOTOR_IN1_PORT   5
#define MOTOR_IN1_PIN    5
#define MOTOR_IN2_PORT   5
#define MOTOR_IN2_PIN    3


#define MOTOR_MIN_SPEED      25      // %
#define MOTOR_MAX_SPEED      90      // %
#define MOTOR_RAMP_STEP      2       // % per update

//Internal State

static uint8_t current_speed = 0;
static uint8_t target_speed  = 0;
static uint8_t IN1_out = 1;
static uint8_t IN2_out = 0; 
static motor_dir_t direction = MOTOR_DIR_FORWARD;

//Helpers

static uint32_t percent_to_compare(uint8_t percent)
{
    return (percent * MOTOR_PWM_PERIOD) / 100;
}

//API

void motor_init(void)
{
    
    /* Init direction GPIO */
    gpio_init(MOTOR_DIR_PORT, MOTOR_DIR_PIN, GPIO_OUTPUT_PUSH_PULL);
    gpio_hsiom_set(MOTOR_DIR_PORT, MOTOR_DIR_PIN, 8);

    gpio_init(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_OUTPUT_PUSH_PULL);
    gpio_init(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_OUTPUT_PUSH_PULL);
    gpio_write(MOTOR_IN1_PORT, MOTOR_IN1_PIN, IN1_out); //set IN1 high
    gpio_write(MOTOR_IN2_PORT, MOTOR_IN2_PIN, IN2_out); //set IN2 low

    clock_div16_init(1, 3, 13); //24Mhz/3 = 8MHz clock to tcpwm7 (peripheral 13)
    pwm_init(7, 800, 0); //initialize pwm on tcpwm7 with period
    pwm_start(7);
    
    current_speed = 0;
    target_speed  = 0;
    direction     = MOTOR_DIR_FORWARD;
}

void motor_set_target_speed(uint8_t percent)
{
    if (percent < MOTOR_MIN_SPEED)
        percent = MOTOR_MIN_SPEED;
    else if (percent > MOTOR_MAX_SPEED)
        percent = MOTOR_MAX_SPEED;

    target_speed = percent;
}

void motor_toggle_direction(void)
{
    if (IN1_out ==1){
        IN1_out =0;
        IN2_out =1;
    }else{
        IN1_out =1;
        IN2_out =0;
    }
        
    gpio_write(MOTOR_IN1_PORT, MOTOR_IN1_PIN, IN1_out);
    gpio_write(MOTOR_IN2_PORT, MOTOR_IN2_PIN, IN2_out);
}

void motor_update(void)
{
    /* Ramp logic */
    if (current_speed < target_speed)
        current_speed += MOTOR_RAMP_STEP;
    else if (current_speed > target_speed)
        current_speed -= MOTOR_RAMP_STEP;

    if (current_speed > MOTOR_MAX_SPEED)
        current_speed = MOTOR_MAX_SPEED;

    /* Update PWM compare value directly */
    TCPWM_CNT_REG(
        MOTOR_PWM_CNT,
        TCPWM_CC_OFFSET
    ) = percent_to_compare(current_speed);

    
}
