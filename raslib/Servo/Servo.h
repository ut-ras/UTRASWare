/**
 * @file Servo.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief High level driver for running a SM-S4303R Springrc servo motor.
 * @version 0.1
 * @date 2021-09-29
 * @copyright Copyright (c) 2021
 */
#pragma once

/** Device Specific imports. */
#include <lib/PWM/PWM.h>


/** @brief ServoConfig_t is a user defined struct that specifies a servo
 *         configuration . */
typedef struct ServoConfig {
    /**
     * @brief Pin servo is connected to.
     *
     * Default M0_PB6.
     */
    PWMPin_t pin;
} ServoConfig_t;

/**
 * @brief ServoInit starts up a motor on a specified PWM pin.
 * 
 * @param config PWMPin_t pin to set up PWM on.
 * @return PWM_t instance that manages the servo motor.
 */
PWM_t ServoInit(PWMPin_t config);


/**
 * @brief ServoSetSpeed sets the servo speed.
 * 
 * @param pwm PWM_t instance that manages the servo motor.
 * @param speed New speed of the servo motor. From [-100, 100].
 */
void ServoSetSpeed(PWM_t pwm, int8_t speed);

/**
 * @brief ServoStart enables the servo movement.
 * 
 * @param servo Servo motor to enable.
 */
void ServoStart(PWM_t servo);

/**
 * @brief ServoStop disables the servo movement.
 * 
 * @param servo Servo motor to disable.
 */
void ServoStop(PWM_t servo);