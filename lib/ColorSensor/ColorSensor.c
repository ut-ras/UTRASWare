/**
 * @file ColorSensor.h
 * @details deigned for TM4C microcontroller. This driver/library allows you to initialize color sensors,
 *  update and read their RGBC values and set up interrupts for each color where you can set a low and 
 *  high data threshold.
 * @brief Low-level driver for TCS34725 color sensor
 * @author Dario Jimenez
 * @date 09/16/21
 * 
 *************************************************************************************************************
 *
 * TCS34725 Color Sensor information:
 *    - 3.3V pin: connect to the +3.3V
 *    - GND pin: connect to ground
 *    - SCL pin: connect to PD0
 *    - SDA pin: connect to PD1
 *    - LED pin: connect to ground if you want LED off
 *    - INT pin: connect to PA7 if activating Clear interrupt
 * 
 **/

/* library imports */
#include <lib/ColorSensor/ColorSensor.h>
#include <inc/I2C3.h>
#include <lib/GPIO/GPIO.h>
#include <lib/Timers/Timers.h>

#define MAX_COLORSENSOR_TIMERS 4 // Max number of interrupts (1 for each color)
#define MAX_COLORSENSOR_FREQ 30 //max frequency for color sensor


static GPIOConfig_t INT_Pin ={PIN_A7, NONE, false, false, 0, 0}; //GPIO configuration for int pin
static TimerConfig_t timers[MAX_COLORSENSOR_TIMERS]; //array of timer of configuration for each color sensor
static ColorSensor_t* interruptSensor; //instance of color sensor being used.

/* interrupt counters for persistance buffer */
static uint8_t redCount = 0;
static uint8_t greenCount = 0;
static uint8_t blueCount = 0;



//indicates if I2C is on
uint8_t isI2COn = 0;

/* values to transmit for setting up color sensor */
static uint8_t EnableWriteValues[2] = {TCS34725_COMMAND | TCS34725_ENABLE_R, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN};
static uint8_t ATimeWriteValues[2] = {TCS34725_COMMAND | TCS34725_ATIME_R, TCS34725_ATIME_10};
static uint8_t ControlWriteValues[2] = {TCS34725_COMMAND | TCS34725_CONTROL_R, TCS34725_CONTROL_1X_GAIN};

/* Values to transmit for setting up clear interrupt */
static uint8_t setInterruptValues[2] = {TCS34725_COMMAND | TCS34725_ENABLE_R, TCS34725_ENABLE_AIEN | TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN};
static uint8_t persistenceValues[2] = { TCS34725_COMMAND | TCS34725_PERS_R, TCS34725_PERS_5CYCLES};


/** 
 * @brief Initializes color sensor to its default settings and I2C interface
 * 
 * @param sensor instance of ColorSensor_t that you want to initialize passed as a pointer
 * 
 * @return value 1 if correctly intialized, value 0 is not
 **/

int ColorSensor_init(ColorSensor_t* sensor){

    /* Activate I2C protocol if not activated already */
    if(isI2COn == 0) {
      I2C3_Init(300000, 80000000); 
      isI2COn = 1;
    }

  uint8_t command = (TCS34725_COMMAND | TCS34725_ID_R); //command byte to indicate to read ID register
  uint8_t ID; //ID of I2C device

  /* check devive ID and return if not correct ID*/
  TransmitAndReceive(TCS34725_ADDRESS, &ID, &command);
  if((ID != TCS34725_ID1) && (ID != TCS34725_ID2)) return 0;
  
  /* set up sensor's configuration */
  I2C3_Send2(TCS34725_ADDRESS, ControlWriteValues[0], ControlWriteValues[1]);
  I2C3_Send2(TCS34725_ADDRESS, ATimeWriteValues[0], ATimeWriteValues[1]);

  /* Enable RGBC ADC and internal TCS34725 oscillator */
  I2C3_Send2(TCS34725_ADDRESS, EnableWriteValues[0], EnableWriteValues[1]);


  /* private sensor configuration */
  sensor->priv.isInitialized = 1; //indicate sensor has been initialized

  /* initialize interrupt flags to 0 */
	sensor->clearInterrupt = 0;
  sensor->redInterrupt = 0;
  sensor->greenInterrupt = 0;
  sensor->blueInterrupt = 0;

  /* indicate that on interrupts have been initialized for any color sensor */
  sensor->priv.isBlueInt = 0; 
  sensor->priv.isClearInt = 0; 
  sensor->priv.isRedInt = 0; 
  sensor->priv.isGreenInt = 0;

  return 1; 

}

/**
 * @brief updates 16-bit values from red, green, blue, and clear ADC color sensors.
 * 
 * @param sensor an instance of ColorSensor_t passed as a pointer
 * 
 * @note If color sensor is not initalized, value won't update
 * @note ClearValue, RedValue, GreenValue, BlueValue variables in sensor will be updated
 **/

void ColorSensor_Read(ColorSensor_t* sensor){

  /* don't read if not initialized */
  if(sensor->priv.isInitialized == 0 || isI2COn == 0) return;

  /* Read 2 bytes from clear sensor */
  uint8_t command = (TCS34725_COMMAND | TCS34725_CDATAH_R); //command for clear high byte
  uint8_t dataL; // low byte
  uint8_t dataH; // high byte
  uint16_t sensorVal;

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_CDATAL_R; // new command for clear low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); //transmit command, receive low byte

  /* combine upper and lower bytes and store it*/
  sensorVal = dataH;
  sensorVal = sensorVal << 8;
  sensorVal |= dataL;
  sensor->ClearValue =  sensorVal; //combine both high and low bytes and store it


  /* Read 2 bytes from Red sensor */
  command = (TCS34725_COMMAND | TCS34725_RDATAH_R); //command for red high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits
  sensorVal &= 0x0000;

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_RDATAL_R; //new command for red low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  /* combine upper and lower bytes and store it*/
  sensorVal = dataH;
  sensorVal = sensorVal << 8;
  sensorVal |= dataL;
  sensor->RedValue = sensorVal; //combine both high and low bytes and store it



  /* Read 2 bytes from Green sensor */
  command = (TCS34725_COMMAND | TCS34725_GDATAH_R); //command for green high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits
  sensorVal &= 0x0000;

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_GDATAL_R; //new command for green low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  /* combine upper and lower bytes and store it*/
  sensorVal = dataH;
  sensorVal = sensorVal << 8;
  sensorVal |= dataL;
  sensor->GreenValue = sensorVal; //combine both high and low bytes and store it


  /* Read 2 bytes from Blue sensor */
  command = (TCS34725_COMMAND | TCS34725_BDATAH_R); //command for blue high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits
  sensorVal &= 0x0000;

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_BDATAL_R; //new command for green low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  /* combine upper and lower bytes and store it*/
  sensorVal = dataH;
  sensorVal = sensorVal << 8;
  sensorVal |= dataL;
  sensor->BlueValue = sensorVal; 
}

/**
 * @brief interrupt handler for clear interrupt 
 **/
static void ColorSensor_Handler0(){
	ColorSensor_Read(interruptSensor);

  if(GPIOGetBit(PIN_A7)) interruptSensor->clearInterrupt = 1;
	
	else interruptSensor->clearInterrupt = 0;
}

/**
 * @brief interrupt handler for green interrupt 
 **/
static void ColorSensor_Handler1(){
  ColorSensor_Read(interruptSensor);

  if(interruptSensor->GreenValue < interruptSensor->priv.greenLow || interruptSensor->GreenValue > interruptSensor->priv.greenHigh){
    greenCount++;
    if(greenCount >= 5){
      interruptSensor->greenInterrupt =1;
      greenCount = 0;
    }
  } 

  else{
    interruptSensor->greenInterrupt = 0;
    greenCount = 0;
  }

}

/**
 * @brief interrupt handler for red interrupt 
 **/
static void ColorSensor_Handler2(){
  ColorSensor_Read(interruptSensor);

  if(interruptSensor->RedValue < interruptSensor->priv.redLow || interruptSensor->RedValue > interruptSensor->priv.redHigh) {
    redCount++;
    if(redCount >= 5){
      redCount = 0;
      interruptSensor->redInterrupt =1;
    }
  }

  else {
    interruptSensor->redInterrupt = 0;
    redCount = 0;}
}

/**
 * @brief interrupt handler for blue interrupt 
 **/
static void ColorSensor_Handler3(){
  ColorSensor_Read(interruptSensor);

  if(interruptSensor->BlueValue < interruptSensor->priv.blueLow || interruptSensor->BlueValue > interruptSensor->priv.blueHigh){
    blueCount++;
    if(blueCount >= 5){
      interruptSensor->blueInterrupt =1;
      blueCount=0;
    }
  } 

  else{
    interruptSensor->blueInterrupt = 0;
    blueCount = 0;
  } 
}

/**
 * @brief setup for clear interrupt
 **/
static void ClearInterrupt_Init(int high, int low){

    I2C3_SendData(TCS34725_ADDRESS, setInterruptValues, 2); //initialize interrupts in device
    I2C3_SendData(TCS34725_ADDRESS, persistenceValues, 2); //set persistance to 5 consecutive cycles

		GPIOInit(INT_Pin);

    /* set low threshold data */

    uint8_t thresholdData[2] = {TCS34725_COMMAND | TCS34725_AILTL_R, low&(0x00FF)}; //data for low threshold lower byte

    I2C3_Send2(TCS34725_ADDRESS, thresholdData[0], thresholdData[1]);

    /* data for low threshold upper byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AILTH_R;
    thresholdData[1] = (low&(0xFF00)) >>8;

    //I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to low threshold upper byte
    I2C3_Send2(TCS34725_ADDRESS, thresholdData[0], thresholdData[1]);

    /* set high threshold data */

    /* data for high threshold lower byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AIHTL_R;
    thresholdData[1] = (high&(0x00FF)); 

    //I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to high threshold lower byte
    I2C3_Send2(TCS34725_ADDRESS, thresholdData[0], thresholdData[1]);

    /* data for high threshold upper byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AIHTH_R;
    thresholdData[1] = (high&(0xFF00)) >> 8;

    I2C3_Send2(TCS34725_ADDRESS, thresholdData[0], thresholdData[1]);
}


/**
 * @brief initializes an interrupt that sets a flag in ColorSensor_t instance indicating a value from a color sensor is greater than high threshold
 *        less than low threshold
 * 
 * @param sensor instance of ColorSensor_t passed as a pointer.
 * @param low low threshold
 * @param high high threshold
 * @param color desired color to set interrupt for
 * @param priority priority of the interrupt
 * @param timerID desired timer for interrupt. Check Timers.h library for a full list of all available timers
 * 
 * @note interrupts will run at 30 Hz
 * @note PA7 GPIO pin will needed if interrupt for clear sensor is initialized and must be connected to "int" pin in color sensor device
 * 
 **/
void ColorSensor_SetInterrupt(ColorSensor_t* sensor, uint16_t low, uint16_t high, ColorSensorColors_t color, uint8_t priority, TimerID_t timerID){

  /* initialize color sensor */
  if(sensor->priv.isInitialized == 0){
    ColorSensor_init(&sensor);
  }

  /* configuration for the interrupt */
  timers[color].timerID = timerID;
  timers[color].isPeriodic = true;
  timers[color].priority = priority;
  timers[color].period = freqToPeriod(MAX_COLORSENSOR_FREQ, MAX_FREQ);

  interruptSensor = sensor; //store sensor

  switch (color)
  {

  /* clear sensor interrupt */
  case CLEAR:
    
    if(interruptSensor->priv.isClearInt == 0){
      ClearInterrupt_Init(high, low);
      timers[color].handlerTask = ColorSensor_Handler0;
    }
    break;
  

  /* green sensor interrupt */
  case GREEN:

    if(interruptSensor->priv.isGreenInt == 0) timers[color].handlerTask = ColorSensor_Handler1;
    sensor->priv.greenHigh = high;
    sensor->priv.greenLow  = low;
    break;

  /* red sensor interrupt */
  case RED:
    if(interruptSensor->priv.isRedInt == 0) timers[color].handlerTask = ColorSensor_Handler2;
    sensor->priv.redHigh = high;
    sensor->priv.redLow  = low;
    break;

  /* blue sensor interrupt */
  case BLUE:
    if(interruptSensor->priv.isBlueInt == 0)timers[color].handlerTask = ColorSensor_Handler3;
    sensor->priv.blueHigh = high;
    sensor->priv.blueLow  = low;
    break;
  }

  TimerInit(timers[color]); //initialize interrupt
}

/**
 * @brief disable a specified interrupt 
 * 
 * @param sensor instance of ColorSensor_t passed as a pointer.
 * 
 * @param color color for the interrupt you want to disable
 * 
 **/

void ColorSensor_DisableInterrupt(ColorSensor_t* sensor, ColorSensorColors_t color){

  switch (color)
  {
  case CLEAR:
    if(sensor->priv.isClearInt == 1) TimerStop(timers[color].timerID);
    break;
  
  case RED:
    if(sensor->priv.isRedInt == 1) TimerStop(timers[color].timerID);
    break;

  case GREEN:
    if(sensor->priv.isGreenInt == 1) TimerStop(timers[color].timerID);
    break;

  case BLUE:
    if(sensor->priv.isBlueInt == 1) TimerStop(timers[color].timerID);
    break;
  }
}




