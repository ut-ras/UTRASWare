/**
 * File name: ColorSensorExample.c
 * Devices: TM4C123
 * Description: Red, blue, or green board LEDs turn on if sensdor detects a red, blue, or green color object respectively. 
 * Authors: Dario Jimenez
 * Last Modified: 10/18/21
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
#include <raslib/ColorSensor/ColorSensor.h>
#include <lib/I2C/I2C.h>
#include <lib/PWM/PWM.h>

#define DEBUG = 0;

/** 
 * These function declarations are defined in the startup.s assembly file for
 * managing interrupts. 
 */
void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

void updateLEDs(uint16_t* redValue, uint16_t* greenValue, uint16_t* blueValue, PWM_t* redPWM, PWM_t* greenPWM, PWM_t* bluePWM) {
	uint16_t totValue = *redValue + *greenValue + *blueValue;
	uint8_t redDuty = 100 - 100 * *redValue / totValue;
	uint8_t greenDuty = 100 - 100 * *greenValue / totValue;
	uint8_t blueDuty = 100 - 100  * *blueValue / totValue;
	PWMUpdateConfig(*redPWM, freqToPeriod(1000, MAX_FREQ), redDuty);
	PWMUpdateConfig(*greenPWM, freqToPeriod(1000, MAX_FREQ), greenDuty);
	PWMUpdateConfig(*bluePWM, freqToPeriod(1000, MAX_FREQ), blueDuty);
}

/** Your main execution loop. */
int main(void) {
	
    /* Clock setup. */
    PLLInit(BUS_80_MHZ);
    DisableInterrupts();
		
		/* configuration for red built-in */
		GPIOConfig_t redLED = {
			.pin=PIN_F1,
			.isOutput=true,
			.pull=GPIO_PULL_DOWN,
			.alternateFunction=5
		};
		
		/* configuration for blue built-in */
		GPIOConfig_t blueLED = {
			.pin=PIN_F2,
			.isOutput=true,
			.pull=GPIO_PULL_DOWN,
			.alternateFunction=5
		};
		
		/* configuration for green built-in */
		GPIOConfig_t greenLED = {
			.pin=PIN_F3,
			.isOutput=true,
			.pull=GPIO_PULL_DOWN,
			.alternateFunction=5
		};
		
		/* Initialize GPIO pins */
		GPIOInit(redLED);
		GPIOInit(blueLED);
		GPIOInit(greenLED);
		
		/* Initialize Red LED to off */
		PWMConfig_t redConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F1,
				.sourceInfo.timerSelect.timerID=TIMER_2A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=100
		};
		PWM_t redPWM = PWMInit(redConfig);
		
		/* Initialize Blue LED to off */
		PWMConfig_t blueConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F2,
				.sourceInfo.timerSelect.timerID=TIMER_3A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=100
		};
		PWM_t bluePWM = PWMInit(blueConfig);
		
		/* Initialize Green LED to off */
		PWMConfig_t greenConfig = {
				.source=PWM_SOURCE_TIMER,
				.sourceInfo.timerSelect.pin=PIN_F3,
				.sourceInfo.timerSelect.timerID=TIMER_4A,
				.period=freqToPeriod(1000, MAX_FREQ),
				.dutyCycle=100
		};
		PWM_t greenPWM = PWMInit(greenConfig);
		
		/* I2C configuration */
    I2CConfig_t i2ccon =  {
				.module=I2C_MODULE_0, // This uses pins PB2 (SCL) and PB3 (SDA).
				.speed=I2C_SPEED_400_KBPS //baud rate of 400 kilobits per second
		};
		
		/* color sensor configuration */
		ColorSensorConfig_t config = {
			.I2CConfig= i2ccon, 
			.isInterrupt=true, //use interrupt 
			.samplingFrequency = 200, //200 hz
			.timerID=TIMER_1A //timer 1A
		};
		
		/* Initialize color sensor */
		ColorSensor_t sensor = ColorSensorInit(config);
		
		/* Timer to signal when to update the LED color */
		void* timerArgs[] = {&sensor.RedValue, &sensor.GreenValue, &sensor.BlueValue, &redPWM, &greenPWM, &bluePWM};
		TimerConfig_t ledTimerConfig = {
			.timerID=TIMER_5A,
			.period=8000000,
			.timerTask=updateLEDs,
			.isPeriodic=true,
			.timerArgs=&timerArgs
		};
		Timer_t ledTimer = TimerInit(ledTimerConfig);
		TimerStart(ledTimer);
		
		EnableInterrupts();
    
    /** Main loop. Put your program here! */
		uint16_t redDebugArray[100];
		uint16_t greenDebugArray[100];
		uint16_t blueDebugArray[100];
		uint8_t debugIndex = 0;
    while (1) {	
			#ifdef DEBUG
			if (debugIndex < 100) {
				redDebugArray[debugIndex] = sensor.RedValue;
				greenDebugArray[debugIndex] = sensor.GreenValue;
				blueDebugArray[debugIndex] = sensor.BlueValue;
				debugIndex += 1;
			}
			#endif
		}
}
