/**
 * File name: Example.h
 * Devices: LM4F120; TM4C123
 * Description: <PROGRAM DESCRIPTION>
 * Authors: <YOUR NAME>
 * Last Modified: <TODAY'S DATE>
 */

/** General imports. These can be declared the like the example below. */
#include <stdlib.h>

/** 
 * Device specific imports. Include files and library files are accessed like
 * the below examples.
 */
#include <inc/PLL.h>
#include <lib/ColorSensor/ColorSensor.h>
#include <inc/I2C3.h>
#include <lib/Timers/Timers.h>

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


/** Your main execution loop. */
int main(void) {
    /** Hardware and software initializations. */

    /* Clock setup. */
    PLL_Init(Bus80MHz);
    DisableInterrupts();
	
		//I2C3_Init(300000, 80000000);

		ColorSensor_t s;
	
    ColorSensor_init(&s);

    EnableInterrupts();
	
		ColorSensor_Read(&s);
		ColorSensor_SetInterrupt(&s, 0, 250, GREEN, 1, TIMER_0A);
		ColorSensor_SetInterrupt(&s, 0, 250, RED, 1, TIMER_1A);
		ColorSensor_SetInterrupt(&s, 0, 250, BLUE, 1, TIMER_2A);
		
	
    
    /** Main loop. Put your program here! */
    while (1) {
			//ColorSensor_Read(&s);
		  
			if(s.greenInterrupt == 1){
				int x = 10;
				x=23;
				s.greenInterrupt = 0;
				ColorSensor_DisableInterrupt(GREEN);
			}
			
			if(s.redInterrupt == 1){
				int y =10;
				y =23;
				s.redInterrupt = 0;
				ColorSensor_DisableInterrupt(RED);
			}
			
			if(s.blueInterrupt == 1){
				int y =10;
				y =23;
				s.blueInterrupt = 0;
				ColorSensor_DisableInterrupt(BLUE);
			}
				
			
    }
}

