/**
 * @file Example.c
 * @author your name (you@domain.com)
 * @brief Example program template for new users.
 * @version 0.1
 * @date 2021-09-23
 * @copyright Copyright (c) 2021
 */

/** General imports. These can be declared the like the example below. */
#include <stdlib.h>

/** 
 * Device specific imports. Include files and library files are accessed like
 * the below examples.
 */
#include <lib/PLL/PLL.h>
#include <lib/GPIO/GPIO.h>
#include <lib/Timer/Timer.h>
#include <raslib/Servo/Servo.h>
#include <lib/PWM/PWM.h>
#include <inc/RegDefs.h>


/** 
 * These function declarations are defined in the startup.s assembly file for
 * managing interrupts. 
 */
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

/** 
 * Put your global variables and function declarations and/or implementations
 * here. 
 */
void heartbeat(uint32_t * args) { GPIOSetBit(PIN_F1, !GPIOGetBit(PIN_F1)); }

/** Your main execution loop. */
int main(void) {
    /** Hardware and software initializations. */

    /* Clock setup. */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    /* Initialize PF1 GPIO (red LED) to flash at 1 Hz. */
    GPIOConfig_t PF1Config = {
        .pin=PIN_F1,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };
    GPIOInit(PF1Config);

    TimerConfig_t heartbeatTimerConfig = {
        .timerID=TIMER_0A,
        .period=freqToPeriod(2, MAX_FREQ),
        .timerTask=heartbeat,
        .isPeriodic=true,
        .priority=6,
        .timerArgs=NULL
    };
    TimerInit(heartbeatTimerConfig);

    ServoConfig_t servo1config = {
        .pin=PIN_B6,
        .timerID = TIMER_0B
    };
    ServoConfig_t servo2config = {
        .pin=PIN_B7,
        .timerID = TIMER_1A
    };

    PWM_t servo1 = ServoInit(servo1config);
    PWM_t servo2 = ServoInit(servo2config);

    EnableInterrupts();
    
    /** Main loop. Put your program here! */
    while (1) {
        WaitForInterrupt();
        ServoSetSpeed(servo1, -100);
        ServoSetSpeed(servo2, -100);
        DelayMillisec(5000);
    }
}
