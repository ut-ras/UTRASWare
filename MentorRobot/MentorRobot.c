/**
 * @file    MentorRobot.c
 * @author  Jared McArthur
 * @date    11/12/2021
 * @brief   main file for the RAS 2021 Robotathon mentor robot
 */

#include <inc/PLL/PLL.h>
#include <Robot.h>

void main(void) {
    // initialize robot
    PLLInit(BUS_80_MHZ);
    Robot_t robot = {IDLE, AVERAGE};

    while (1) {
        // when button is pressed, change mode
        
    }
}

