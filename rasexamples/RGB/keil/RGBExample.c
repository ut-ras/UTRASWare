/**
 * @file RGBExample.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief TM4C goes wild with RGB lighting.
 * @version 0.1
 * @date 2021-09-27
 * @copyright Copyright (c) 2021
 * @note Warning to users who have epilepsy - bright flashing colors.
 */

/** General imports. These can be declared the like the example below. */
#include <stdlib.h>

/** 
 * Device specific imports. Include files and library files are accessed like
 * the below examples.
 */
#include <lib/PLL/PLL.h>
#include <lib/PWM/PWM.h>
#include <raslib/RGB/RGB.h>

/** 
 * These function declarations are defined in the startup.s assembly file for
 * managing interrupts. 
 */
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

void PWM_LED(void) {
		GPIOConfig_t pf1 = { // RED
        PIN_F1,
        GPIO_PULL_DOWN,
        true,
        5,
        false,
        GPIO_DRIVE_2MA,
        false
    };

    GPIOConfig_t pf2 = { // BLUE
        PIN_F2,
        GPIO_PULL_DOWN,
        true,
        5,
        false,
        GPIO_DRIVE_2MA,
        false
    };

    GPIOConfig_t pf3 = { // GREEN
        PIN_F3,
        GPIO_PULL_DOWN,
        true,
        5,
        false,
        GPIO_DRIVE_2MA,
        false
    };

    GPIOInit(pf1);
    GPIOInit(pf2);
    GPIOInit(pf3);
	
		PWMConfig_t redConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F1,
				.sourceInfo.timerSelect.timerID=TIMER_0A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=67
		};
		PWMInit(redConfig);
		PWMConfig_t greenConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F3,
				.sourceInfo.timerSelect.timerID=TIMER_1A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=100
		};
		PWMInit(greenConfig);
		PWMConfig_t blueConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F2,
				.sourceInfo.timerSelect.timerID=TIMER_2A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=80
		};
		PWMInit(blueConfig);
}

int main(void) {
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    /* Warning to users who have epilepsy - bright flashing colors. */
    //RGBInit();

		PWM_LED();
	
    EnableInterrupts();
	
		while(1) {};
	
    while(1) {
        WaitForInterrupt();
    }
}
