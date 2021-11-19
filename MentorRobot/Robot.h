/**
 * @file Robot.h
 * @author Jared McArthur
 * @brief 
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>
#include <raslib/ColorSensor/ColorSensor.h>
#include <raslib/DistanceSensor/DistanceSensor.h>
#include <raslib/LineSensor/LineSensor.h>
#include <lib/PWM/PWM.h>

enum events {
    IDLE,
    PAC_MAN,
    DANCE_FLOOR,
    SKEE_BALL
};

enum intelligenceSettings {
    MENTALLY_HANDICAPPED,
    STUPID,
    AVERAGE,
    SMART,
    MATTHEW
};

typedef struct Robot_t {
    int event;
    int intelligence;
    ColorSensor_t colorSensor;
    DistanceSensor_t leftDistanceSensor;
    DistanceSensor_t rightDistanceSensor;
    LineSensor_t leftLineSensor;
    LineSensor_t middleLineSensor;
    LineSensor_t rightLineSensor;
    PWM_t leftMotor;
    PWM_t rightMotor;
    PWM_t flyWheel;
    Servo_t yawServo;
} Robot_t;

/**
 * @brief Set the Event object
 * 
 * @param robot 
 * @param event 
 */
void setEvent(Robot_t robot, uint8_t event);

/**
 * @brief Set the Intelligence object
 * 
 * @param robot 
 * @param intelligence 
 */
void setIntelligence(Robot_t robot, uint8_t intelligence);

/**
 * @brief Get the Color Sensor Red Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getColorSensorRedValue(Robot_t robot);

/**
 * @brief Get the Color Sensor Green Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getColorSensorGreenValue(Robot_t robot);

/**
 * @brief Get the Color Sensor Blue Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getColorSensorBlueValue(Robot_t robot);

/**
 * @brief Get the Color Sensor Clear Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getColorSensorClearValue(Robot_t robot);

/**
 * @brief Get the Left Distance Sensor Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getLeftDistanceSensorValue(Robot_t robot);

/**
 * @brief Get the Right Distance Sensor Value object
 * 
 * @param robot 
 * @return uint16_t 
 */
uint16_t getRightDistanceSensorValue(Robot_t robot);

uint16_t getleftLineSensorValue(Robot_t robot);

uint16_t getMiddleLineSensor(Robot_t robot);

uint16_t getRightLineSensor(Robot_t robot);

void setLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction);

void incrementLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees);

void setRightMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction);

void incrementRightMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees);

void setFlyWheel(Robot_t robot, uint8_t dutyCycle, int8_t direction);

void incrementLeftMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees);

void yawMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction);

void incrementYawMotor(Robot_t robot, uint8_t dutyCycle, int8_t direction, uint8_t degrees);

