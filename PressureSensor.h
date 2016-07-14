#ifndef H_PRESSURE_SENSOR
#define H_PRESSURE_SENSOR

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

#include <stdint.h>
#include <Wire.h>
#include <Arduino.h>

//TODO Prevent lockup if 

class PressureSensor 
{
public:
    PressureSensor();

    void calibrate();
    
    uint32_t getPressure();

    float getAltitude();
private:
    void calculateTemperatureData();
    uint32_t calculatePressure();

    int16_t readInt(uint8_t address);
    int8_t readByte(uint8_t address);

    uint32_t readUnadjustedPressure();
    int16_t readUnadjustedTemperature();

    //Returns false if the i2c timeout period has been reached since the last
    //startTimeoutCounter() call
    void waitForI2C(uint8_t expectedBytes);
    bool unresponsiveI2C;


    static const uint8_t OSS = 2;

    static const uint32_t I2C_TIMEOUT = 50; //Milliseconds

    int16_t ac1;
    int16_t ac2;
    int16_t ac3;
    uint16_t ac4;
    uint16_t ac5;
    uint16_t ac6;
    int16_t b1;
    int16_t b2;
    int16_t mb;
    int16_t mc;
    int16_t md;

    int32_t b5;

    float i2cStartTime;

    //The pressure at sea level. Value set in .cpp file
    static const float SEA_PRESSURE;
};
#endif
