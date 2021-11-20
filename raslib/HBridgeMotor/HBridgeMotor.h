/**
 * @file HBridgeMotor.h
 * @author Jared McArthur
 * @brief High level driver for an h-bridge motor.
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <lib/PWM/PWM.h>
#include <lib/GPIO/GPIO.h>

/** @brief HBridgeMotor_t is a user defined struct that specifies a h-bridge
 *         motor configuration . */
typedef struct HBridgeMotorConfig {
    /** 
     * @brief Pin In1 is connected.
     * 
     * Default PIN_A0.
     */
    GPIOPin_t in1Pin;

    /**
     * @brief Pin In2 is connected.
     * 
     * Default PIN_A0
     */
    GPIOPin_t in2Pin;

    /**
     * @brief Pin h-bridge motor is connected to.
     *
     * Default M0_PB6.
     */
    PWMPin_t pwmPin;
} HBridgeMotorConfig_t;

typedef struct HBridgeMotor {

    /**
     * @brief Pin In1 is connected to.
     * 
     */
    GPIOPin_t in1Pin;
    
    /**
     * @brief Pin In2 is connected to.
     * 
     */
    GPIOPin_t in2Pin;
    
    /**
     * @brief H-bridge's PWM that controls the motor speed.
     * 
     */
    PWM_t pwm;
} HBridgeMotor_t;

/**
 * @brief HBridgeMotorInit starts up a motor on a specified PWM pin.
 * 
 * @param config HBridgeMotorConfig_t to set up.
 * @return PWM_t instance that manages the h-bridge motor.
 */
 HBridgeMotor_t HBridgeMotorInit(HBridgeMotorConfig_t config);

/**
 * @brief ServoSetSpeed sets the servo speed.
 * 
 * @param motor HBridgeMotor_t instance that manages the h-bridge motor.
 * @param speed New speed of the h-bridge motor. From [-100, 100].
 */
void HBridgeMotorSetSpeed(HBridgeMotor_t motor, int8_t speed);

/**
 * @brief HBridgeMotorStart enables the motor movement.
 * 
 * @param motor H-bridge motor to enable.
 */
void HBridgeMotorStart(HBridgeMotor_t motor);

/**
 * @brief HBridgeMotorStop disables the motor movement.
 * 
 * @param motor H-bridge motor to disable.
 */
void HBridgeMotorStop(HBridgeMotor_t motor);

