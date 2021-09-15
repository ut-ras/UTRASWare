
#ifndef ColorSensor_H
#define ColorSensor_H

#include <stdint.h>


#define TCS34725_ADDRESS 0x29 //I2C Slave Adress for TCS34725

#define TCS34725_COMMAND 0x80 //Command bit for command register
#define TCS34725_ENABLE_PON 0x01 //Power On bit on Enable Register: writing 1 activates internal oscillator, 0 disables it
#define TCS34725_ENABLE_AEN 0x02 //RGBC Enable bit on Enable Register. 1: enable TCS34725 ADC. 0: disable TCS34725 ADC
#define TCS34725_ENABLE_WEN 0x08 //Wait Enable bit on Enable Register. 1: activates wait timer. 0: disables it.
#define TCS34725_ENABLE_AIEN 0x10 //RGBC Interrupt Inable bit on Enable Register. 1: permits interrupts. 0: does not permit interrupt
#define TCS34725_ATIME_10 0xF6
#define TCS34725_CONTROL_1X_GAIN 0x00

#define TCS34725_ID1 0x44 //TCS34725 and TCS34721 ID number
#define TCS34725_ID2 0x4D //TCS34723 and TCS34727 ID number


/* Registeraddresses for color sensor */ 
#define TCS34725_ENABLE_R 0x00
#define TCS34725_ATIME_R 0x01
#define TCS34725_WTIME_R 0x03
#define TCS34725_AILTL_R 0x04
#define TCS34725_AILTH_R 0x05
#define TCS34725_AIHTL_R 0x06
#define TCS34725_AIHTH_R 0x07
#define TCS34725_PERS_R 0x0C
#define TCS34725_CONFIG_R 0x0D
#define TCS34725_CONTROL_R 0x0F
#define TCS34725_ID_R 0x12
#define TCS34725_STATUS_R 0x13
#define TCS34725_CDATAL_R 0x14
#define TCS34725_CDATAH_R 0x15
#define TCS34725_RDATAL_R 0x16
#define TCS34725_RDATAH_R 0x17
#define TCS34725_GDATAL_R 0x18
#define TCS34725_GDATAH_R 0x19
#define TCS34725_BDATAL_R 0x1A
#define TCS34725_BDATAH_R 0x1B

typedef struct PrivateColorSensor{
    int isInitialized;
}PrivateColorSensor_t;

typedef struct ColorSensor{
    uint16_t RedValue;
    uint16_t GreenValue;
    uint16_t BlueValue;
    uint16_t ClearValue;
    PrivateColorSensor_t priv;
}ColorSensor_t;

int ColorSensor_init(ColorSensor_t* sensor);

void ColorSensor_Read(ColorSensor_t* sensor);

void ColorSensor_setInterrupt(ColorSensor_t* sensor);

#endif