
#include <lib/ColorSensor/ColorSensor.h>
#include<inc/I2C3.h>
#include <lib/GPIO/GPIO.h>
#include <lib/Timers/Timers.h>

static GPIOConfig_t INT ={PIN_A7, NONE, false, false, 0, 0};
static TimerConfig_t timer;
static ColorSensor_t * interruptSensor;



uint8_t isI2COn = 0;

//TODO: set a time delay for integration to happen

uint8_t EnableWriteValues[2] = {TCS34725_COMMAND | TCS34725_ENABLE_R, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN};
uint8_t ATimeWriteValues[2] = {TCS34725_COMMAND | TCS34725_ATIME_R, TCS34725_ATIME_10};
uint8_t ControlWriteValues[2] = {TCS34725_COMMAND | TCS34725_CONTROL_R, TCS34725_CONTROL_1X_GAIN};
void Send(uint8_t addr, uint8_t* pdata){

}

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
  I2C3_SendData(TCS34725_ADDRESS, ControlWriteValues, 2); //write to control register to set the gain to x1
  I2C3_SendData(0x29, ATimeWriteValues, 2); //write to RGBC Timing Register and set the ADC timing to 10 cycles
  
  /* Enable RGBC ADC and internal TCS34725 oscillator */
  I2C3_SendData(0x29, EnableWriteValues, 2);

  sensor->priv.isInitialized = 1; //indicate sensor has been initialized

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

//TODO: check if function works 
void ColorSensor_Read(ColorSensor_t* sensor){

  if(sensor->priv.isInitialized == 0 || isI2COn == 0) return;
	
	

  /* Read 2 bytes from clear sensor */
  uint8_t command = (TCS34725_COMMAND | TCS34725_CDATAH_R); //command for clear high byte
  uint8_t dataL; // low byte
  uint8_t dataH; // high byte

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_CDATAL_R; // new command for clear low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); //transmit command, receive low byte

  dataH = dataH << 8; //shift high byte 8 bits up
  sensor->ClearValue = (dataH | dataL); //combine both high and low bytes and store it



  /* Read 2 bytes from Red sensor */
  command = (TCS34725_COMMAND | TCS34725_RDATAH_R); //command for red high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_RDATAL_R; //new command for red low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  dataH = dataH << 8; //shift high byte 8 bits up
  sensor->RedValue = (dataH | dataL); //combine both high and low bytes and store it



  /* Read 2 bytes from Green sensor */
  command = (TCS34725_COMMAND | TCS34725_GDATAH_R); //command for green high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_GDATAL_R; //new command for green low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  dataH = dataH << 8; //shift high byte 8 bits up
  sensor->GreenValue = (dataH | dataL); //combine both high and low bytes and store it


  /* Read 2 bytes from Blue sensor */
  command = (TCS34725_COMMAND | TCS34725_BDATAH_R); //command for blue high byte
  dataL &= 0x0; //clear all bits
  dataH &= 0x0; //clear all bits

  TransmitAndReceive(TCS34725_ADDRESS, &dataH, &command); //transmit command, receive high byte
  command = TCS34725_COMMAND | TCS34725_BDATAL_R; //new command for green low byte
  TransmitAndReceive(TCS34725_ADDRESS, &dataL, &command); // transmit command, receive low byte

  dataH = dataH << 8; //shift high byte 8 bits up
  sensor->BlueValue = (dataH | dataL); //combine both high and low bytes and store it
}


uint8_t setInterruptValues[2] = {TCS34725_COMMAND | TCS34725_ENABLE_R, TCS34725_ENABLE_AIEN | TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN};
uint8_t persistenceValues[2] = { TCS34725_COMMAND | TCS34725_PERS_R, TCS34725_PERS_5CYCLES};


static void ColorHandler(){
	ColorSensor_Read(interruptSensor);
  if(GPIOGetBit(PIN_A7)) interruptSensor->interrupt = 1;
}
void ColorSensor_SetInterrupt(ColorSensor_t* sensor, uint16_t low, uint16_t high, ColorSensorColors_t color, uint8_t priority){

  /* configuration for the interrupt */
  timer.timerID = TIMER_0A;
  timer.handlerTask =  ColorHandler;
  timer.isPeriodic = true;
  timer.priority = priority;
  timer.period = freqToPeriod(90, MAX_FREQ);

  interruptSensor = sensor;

  switch (color)
  {
  case CLEAR:
    
    I2C3_SendData(TCS34725_ADDRESS, setInterruptValues, 2); //initialize interrupts in device
    I2C3_SendData(TCS34725_ADDRESS, persistenceValues, 2); //set persistance to 5 consecutive cycles

		GPIOInit(INT);

    /* set low threshold data */

    uint8_t thresholdData[2] = {TCS34725_COMMAND | TCS34725_AILTL_R, low&(0x0F)}; //data for low threshold lower byte

    I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to low threshold lower byte

    /* data for low threshold upper byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AILTH_R;
    thresholdData[0] = low&(0xF0);

    I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to low threshold upper byte


    /* set high threshold data */

    /* data for high threshold lower byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AIHTL_R;
    thresholdData[1] = high&(0xF0); 

    I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to high threshold lower byte

    /* data for high threshold upper byte */
    thresholdData[0] = TCS34725_COMMAND | TCS34725_AIHTH_R;
    thresholdData[1] = high&(0xF0);

    I2C3_SendData(TCS34725_ADDRESS, thresholdData, 2); //write to high threshold upper byte

    TimerInit(timer); //initialize interrupt
    




    break;
  
  default:
    break;
  }

  
}