/**
 * @file PWMExample.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief An example project showing how to use the PWM driver.
 * @version 0.1
 * @date 2021-09-23
 * @copyright Copyright (c) 2021
 * @note
 * Modify __MAIN__ on L13 to determine which main method is executed.
 * __MAIN__ = 0 - Initialization and management of a timer acting as a PWM for low freq.
 *          = 1 - Initialization and management of a PWM module for high freq.
 */
#define __MAIN__ 0

/** General imports. */
#include <stdlib.h>

/** Device specific imports. */
#include <lib/PLL/PLL.h>
#include <lib/GPIO/GPIO.h>
#include <lib/Timer/Timer.h>
#include <lib/PWM/PWM.h>


void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

#if __MAIN__ == 0
int main(void) {
    /**
     * This program demonstrates initializing initializing the TM4C PWM module,
     * flashing an LED with it, updating its frequency and duty cycle, and then
     * stopping it.
     */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    /* Initialize SysTick for delay calls.*/
    DelayInit();
    
    PWMConfig_t pwmConfigPF1 = {
        .source=PWM_SOURCE_TIMER,
        .sourceInfo={
            .timerSelect={
                .pin=PIN_F1,
                .timerID=TIMER_0A
            }
        },
		.period=freqToPeriod(2, MAX_FREQ),
		.dutyCycle=50
    };

	/* The LED connected to PF1 should flash at 2 Hz with even on-off times. */
    PWM_t pwm = PWMInit(pwmConfigPF1);

    EnableInterrupts();
    uint8_t mode = 3;
    while (1) {
        switch (mode) {
            case 0:
                /* The LED connected to PF1 should flash at 2 Hz with short on and long off times. */
                PWMUpdateConfig(pwm, freqToPeriod(2, MAX_FREQ), 12);
                break;
            case 1:
                /* The LED connected to PF1 should flash at 5 Hz with even on-off times. */
                PWMUpdateConfig(pwm, freqToPeriod(5, MAX_FREQ), 50);
                break;
            case 2:
                /* The LED connected to PF1 should stop in the on position. */
                PWMStop(pwm);
                GPIOSetBit(PIN_F1, 1);
                break;
            case 3:
                /* The LED connected to PF1 should start. */
                PWMStart(pwm);
                break;
        }
        DelayMillisec(2000);
        mode = (mode + 1) % 4;
    }
}

#elif __MAIN__ == 1
int main(void) {
    /**
     * This program demonstrates initializing initializing the TM4C PWM module,
     * and progressively changing the brightness using the duty cycle.
     */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();

    /* Initialize SysTick for delay calls.*/
    delayInit();
    
    PWMConfig_t pwmConfigPF1 = {
        .source=DEFAULT,
        .config={
            .pwmPin=M1_PF1
        }
    };
    PWMInit(pwmConfigPF1, freqToPeriod(2000, MAX_FREQ), 0);

    EnableInterrupts();
    uint8_t dutyCycle = 0;
    while(1) {
        delayMillisec(100);
        /**
         * Run this on an oscilloscope to check if the waveform matches 2 kHz.
         * What if you change the frequency? Duty cycle?
         * At what minimum frequency/period can you load onto the pin and still
         * have an accurate waveform?
         */
        PWMUpdateConfig(pwmConfigPF1, freqToPeriod(2000, MAX_FREQ), dutyCycle);
        dutyCycle = (dutyCycle + 1)%100;
    }
}
#endif
