/**
 * @file Servo.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief High level driver for running a SM-S4303R Springrc servo motor.
 * @version 0.1
 * @date 2021-11-09
 * @copyright Copyright (c) 2021
 */

/** General imports. */
#include <assert.h>

/** Device Specific imports. */
#include <raslib/Servo/Servo.h>


PWM_t ServoInit(ServoConfig_t config) {
    PWMConfig_t pwmConfig = {
        .source=PWM_SOURCE_TIMER,
        .sourceInfo.timerSelect.pin=config.pin,
        .sourceInfo.timerSelect.timerID=config.timerID,
        .sourceInfo.timerSelect.period=240000, // 3 ms
        .dutyCycle=50
    };
    return PWMInit(pwmConfig);
}

void ServoSetSpeed(PWM_t servo, int8_t speed) {
    assert(-100 <= speed && speed <= 100);

    /* 120 000 cycles at 80 MHZ is 1.5 ms pulse width, where the servo moves
       neither clockwise or counter clockwise. 40 000 cycles is 0.5 ms.

       The documentation is not great, but it's somewhat implied that the
       range of pulse width is 1 ms to 2 ms, the former going CCW and the latter
       going CW. 

       We should scale the input speed from [-100, 100] to [1ms, 2ms].
       In this case, the minimum duty cycle to reach 1ms is 33, and the maximum
       duty cycle to reach 2ms is 67, so our mapping is from [-100, 100] -> [33,
       67].  */

    uint8_t dutyCycle = (uint8_t)(50.0 + 0.165 * speed); 
    struct PWMTimerConfig timerConfig = {
       .pin=servo.sourceInfo.timerInfo.pin,
       .timerID=servo.sourceInfo.timerInfo.timer.timerID,
       .period=servo.sourceInfo.timerInfo.timer.period,
       .isIndividual=false,
       .prescale=0
    };
    PWMConfig_t newConf = {
       .source=servo.source,
       .sourceInfo.timerSelect=timerConfig,
       .dutyCycle=dutyCycle
    };
    PWMInit(newConf);
}

void ServoStart(PWM_t servo) {
    PWMStart(servo);
}

void ServoStop(PWM_t servo) {
    PWMStop(servo);
}
