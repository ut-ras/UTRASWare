/**
 * QTRSensor.h
 * Devices: TM4C123, QTR Reflectance Sensor
 * Description: driver for QTR Reflectance Sensor on the TM4C123 board
 * Authors: Dario Jimenez
 * Last Modified: 04/23/2021
 * 
 * QTR reflectance Sensor information:
 * This sensor detects the reflectance of an object.
 * For instance: white reflects a lot of light, so the sensor will detect a white object as high reflectance.
 * In the other hand, black reflects little light, so sensor will detect low reflectance.
 * 
 * Hardware:
 * Connect VCC to 3.3V or 5V
 * Connect Analog pin to either pin 1,2, or 3 in sensor
 * Objects have to be VERY close to the sensor in order for it to work properly
 * 
 * Summary of Driver:
 * This driver can initialize the QTR sensor on a analog pin on the TM4C123 board
 * and returns the raw data from the sensor
 * */


#ifndef QTRSensor_H
#define QTRSensor_H

#include <stdint.h>

/* number of total pins in QTR-8A Reflectance Sensor array */
#define MAX_PINS_QTR_8 8 
/* number of total pins in QTR-8A Reflectance Sensor array */
#define MAX_PINS_QTR_3 3

/* Data Structure that holds ann array of the pins being used and their corresponding values */
typedef struct LineSensorData{
	SensorPin_t pins[MAX_PINS_QTR_8];
	uint16_t SensorValues[MAX_PINS_QTR_8];
	uint8_t numPins; 
}LineSensorData_t;


/* Analog pins that are available for the sensors 
 * When initializing sensor, type in a port name (such as PE3 or PB5) as the input argument */
typedef enum{
	PE3 = 0,
	PE2 = 1,
	PE1 = 2,
	PE0 = 3,
	PD3 = 4,
	PD2 = 5,
	PD1 = 6,
	PD0 = 7,
	PB4 = 10,
	PB5 = 11
}SensorPin_t;

/* indicates if there was an error initializing the Line sensor pins */
typedef enum{
	ERROR = 1,
	NO_ERROR = 0
}Error_t;


Error_t LineSensor_Init(LineSensorData_t lineSensor, uint8_t numPins);

void Read_LineSensor(LineSensorData_t* lineSensor);

/* initializes sensor on desired and eligible pin */
void QTRSensorInit(SensorPin_t pin);

//reads value (0-4095) from the sensor
uint32_t readQTRSensor(void);

#endif