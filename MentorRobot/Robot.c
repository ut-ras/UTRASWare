/**
 * @file Robot.c
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Robot.h>

void initializePacMan(Robot_t robot) {
    // initialize wheel motors
    // initialize distance censors
}

void initializeDanceFloor(void) {
    // initialize wheel motors
    // initialize line followers
    // initialize color sensor
}

void initializeSkeeBall(void) {
    // initialize wheel motors
    // initialize servo
    // initialize flywheel
}

void setEvent(Robot_t robot, uint8_t event) {
    robot.event = event;
    if (event == PAC_MAN) {
        initializePacMan(robot);
    }
    else if (event == DANCE_FLOOR) {
        initializeDanceFloor(robot);
    }
    else if (event == SKEE_BALL) {
        initializeSkeeBall(robot);
    }
}

void setIntelligence(Robot_t robot, uint8_t intelligence) {
    robot.intelligence = intelligence;
}

uint16_t getColorSensorValue(Robot_t robot) {
    return robot.colorSensor.RedValue;
}

uint16_t getColorSensorGreenValue(Robot_t robot) {
    return robot.colorSensor.GreenValue;
}

uint16_t getColorSensorBlueValue(Robot_t robot) {
    return robot.colorSensor.BlueValue;
}

uint16_t getColorSensorClearValue(Robot_t robot) {
    return robot.colorSensor.ClearValue;
}

uint16_t getLeftDistanceSensorValue(Robot_t robot) {
    return robot.leftDistanceSensor.value;
}

uint16_t getRightDistanceSensorValue(Robot_t robot) {
    return robot.rightDistanceSensor.value;
}

uint16_t getleftLineSensorValue(Robot_t robot) {
    return robot.leftLineSensor.value;
}

uint16_t getMiddleLineSensor(Robot_t robot) {
    return robot.middleLineSensor.value;
}

uint16_t getRightLineSensor(Robot_t robot) {
    return robot.rightLineSensor.value;
}

void setLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction) {
    
}

void incrementLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees) {

}

void setRightMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction) {

}

void incrementRightMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees) {

}

void setFlyWheel(Robot_t robot, uint8_t dutyCycle, int8_t direction) {

}

void incrementLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees) {

}

void yawMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction) {
    
}

void incrementYawMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees) {

}

