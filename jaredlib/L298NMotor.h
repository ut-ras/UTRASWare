/**
 * @file L298NMotor.h
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stdint.h>
#include <lib/PWM/PWM.h>
#include <lib/GPIO/GPIO.h>

typedef struct L298NMotor_t {
    PWM_t pwm;
    uint8_t direction;
    GPIOPin_t in1;
    GPIOPin_t in2;
} L298Motor_t;

L298NMotor_t initializeL298NMotor(void);

void setL298NMotorDutyCycle(L298NMotor_t motor, uint8_t dutyCycle);

void setL298NMotorDirection(L298NMotor_t motor, uint8_t direction);

