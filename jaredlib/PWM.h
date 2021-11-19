/**
 * @file PWM.h
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>

#define PWM0_BASE           0x40028000
#define PWM1_BASE           0x40029000
#define PWM_OFFSET          0x040
#define PWM_ENABLE_OFFSET   0x008
#define PWM_CTL_OFFSET      0x040
#define PWM_LOAD_OFFSET     0x050
#define PWM_CMPA_OFFSET     0x058
#define PWM_CMPB_OFFSET     0x05C
#define PWM_GENA_OFFSET     0x060
#define PWM_GENB_OFFSET     0x064

typedef enum PWMPin {
    M0_PB6,
    M0_PB7,
    M0_PB4,
    M0_PB5,
    M0_PE4,
    M0_PE5,
    M0_PC4,
    M0_PD0,
    M0_PC5,
    M0_PD1,
    M1_PD0,
    M1_PD1,
    M1_PA6,
    M1_PE4,
    M1_PA7,
    M1_PE5,
    M1_PF0,
    M1_PF1,
    M1_PF2,
    M1_PF3,
    PWM_COUNT
} PWMPin_t;

typedef struct PWM {
    PWMPin_t pin;
    uint64_t period;
    uint8_t divisor;
} PWM_t;

PWM_t initializePWM(PWMPin_t pin, uint64_t period, uint8_t divisor, uint8_t dutyCycle);

void startPWM(PWM_t pwm);

void stopPWM(PWM_t pwm);

