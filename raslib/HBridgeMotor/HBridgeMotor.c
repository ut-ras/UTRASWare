/**
 * @file HBridgeMotor.c
 * @author Jared McArthur
 * @brief High level driver for an h-bridge motor.
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <assert.h>

#include <raslib/HBridgeMotor/HBridgeMotor.h>

HBridgeMotor_t HBridgeMotorInit(HBridgeMotorConfig_t config) {
    GPIOConfig_t in1Config = {
        .pin=config.in1Pin,
        .isOutput=true
    };
    GPIOConfig_t in2Config = {
        .pin=config.in2Pin,
        .isOutput=true
    };
    PWMConfig_t pwmConfig = {
        .source=PWM_SOURCE_DEFAULT,
        .sourceInfo.pwmSelect.pin=config.pwmPin,
        .sourceInfo.pwmSelect.period=3750, // 3 ms
        .sourceInfo.pwmSelect.divisor=PWM_DIV_2,
        .dutyCycle=50
    };
    HBridgeMotor_t motor = {
        .in1Pin=GPIOInit(in1Config),
        .in2Pin=GPIOInit(in2Config),
        .pwm=PWMInit(pwmConfig)
    };
    return motor;
}

void HBridgeMotorSetSpeed(HBridgeMotor_t motor, int8_t speed) {
    assert(-100 <= speed && speed <= 100);

    uint8_t dutyCycle = speed * (-1 * (bool)(speed & 0x80) + 1 * !((bool)(speed & 0x80)));

    uint8_t val = !((bool)(speed & 0x80));
    uint8_t val1 = ((bool)(speed & 0x80));
    
    GPIOSetBit(motor.in1Pin, !((bool)(speed & 0x80)));
    GPIOSetBit(motor.in2Pin, (bool)(speed & 0x80));
    PWMConfig_t pwmConfig = {
        .source=PWM_SOURCE_DEFAULT,
        .sourceInfo.pwmSelect.pin=motor.pwm.sourceInfo.pin,
        .sourceInfo.pwmSelect.period=3750, // 3 ms
        .sourceInfo.pwmSelect.divisor=PWM_DIV_2,
        .dutyCycle=dutyCycle
    };
    PWMInit(pwmConfig);
}

void HBridgeMotorStart(HBridgeMotor_t motor) {
    PWMStart(motor.pwm);
}

void HBridgeMotorStop(HBridgeMotor_t motor) {
    GPIOSetBit(motor.in1Pin, 0);
    GPIOSetBit(motor.in2Pin, 0);
    PWMStop(motor.pwm);
}

