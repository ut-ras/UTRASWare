/**
 * @file HBridgeMotorExample.c
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <lib/PLL/PLL.h>
#include <raslib/HBridgeMotor/HBridgeMotor.h>

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s

int main(void) {
    PLLInit(BUS_80_MHZ);
    DelayInit();
    DisableInterrupts();

    HBridgeMotorConfig_t motorConfig = {
        .in1Pin=PIN_A2,
        .in2Pin=PIN_A3,
        .pwmPin=M0_PB4
    };

    HBridgeMotor_t motor = HBridgeMotorInit(motorConfig);
    EnableInterrupts();
    
    while (1) {
        HBridgeMotorSetSpeed(motor, 50);
        HBridgeMotorStart(motor);
        DelayMillisec(1000);
        HBridgeMotorStop(motor);
        HBridgeMotorSetSpeed(motor, -50);
        HBridgeMotorStart(motor);
        DelayMillisec(1000);
        HBridgeMotorStop(motor);
    }
}

