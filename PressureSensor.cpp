#include "PressureSensor.h"

const float PressureSensor::SEA_PRESSURE = 101325;

PressureSensor::PressureSensor()
{
    unresponsiveI2C = false;
}

void PressureSensor::calibrate()
{
    ac1 = readInt(0xAA);
    ac2 = readInt(0xAC);
    ac3 = readInt(0xAE);
    ac4 = readInt(0xB0);
    ac5 = readInt(0xB2);
    ac6 = readInt(0xB4);
    b1 = readInt(0xB6);
    b2 = readInt(0xB8);
    mb = readInt(0xBA);
    mc = readInt(0xBC);
    md = readInt(0xBE);
}
uint32_t PressureSensor::getPressure()
{
    if(unresponsiveI2C)
    {
        Serial.println("I2C is unresponsive");
        return 0;
    }

    calculateTemperatureData();


    return calculatePressure();
}

float PressureSensor::getAltitude()
{
    uint32_t pressure = getPressure();

    return (float)44330 * (1-pow(((float)pressure/SEA_PRESSURE), 0.190295));
}

///////////////////////////////////////////////////////////
//                  Private members
///////////////////////////////////////////////////////////

void PressureSensor::calculateTemperatureData()
{
    int32_t ut = readUnadjustedTemperature();

    int32_t x1, x2;

    x1 = (((int32_t)ut - (int32_t)ac6)*(int32_t)ac5) >> 15;
    x2 = ((int32_t)mc << 11)/(x1 + md);
    b5 = x1 + x2;

    //return ((b5 + 8)>>4);  
}
uint32_t PressureSensor::calculatePressure()
{
    uint32_t up = readUnadjustedPressure();

    int32_t x1, x2, x3, b3, b6, p;
    uint32_t b4, b7;

    b6 = b5 - 4000;
    // Calculate B3
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((int32_t)ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (uint32_t)(x3 + 32768))>>15;

    b7 = ((uint32_t)(up - b3) * (50000>>OSS));
    if (b7 < 0x80000000)
        p = (b7<<1)/b4;
    else
        p = (b7/b4)<<1;

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;


    return p;
}

int16_t PressureSensor::readInt(uint8_t address)
{
    uint8_t msb, lsb;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BMP085_ADDRESS, 2);

    waitForI2C(2);

    msb = Wire.read();
    lsb = Wire.read();

    return (int16_t) msb<<8 | lsb;
}
int8_t PressureSensor::readByte(uint8_t address)
{
    uint8_t data;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BMP085_ADDRESS, 1);

    waitForI2C(1);

    return Wire.read();
}

uint32_t PressureSensor::readUnadjustedPressure()
{
    uint8_t msb, lsb, xlsb;
    uint32_t up = 0;

    // Write 0x34+(OSS<<6) into register 0xF4
    // Request a pressure reading w/ oversampling setting
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x34 + (OSS<<6));
    Wire.endTransmission();

    // Wait for conversion, delay time dependent on OSS
    delay(2 + (3<<OSS));

    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF6);
    Wire.endTransmission();
    Wire.requestFrom(BMP085_ADDRESS, 3);

    waitForI2C(3);

    msb = Wire.read();
    lsb = Wire.read();
    xlsb = Wire.read();

    up = (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);

    return up;
}

int16_t PressureSensor::readUnadjustedTemperature()
{
    uint16_t ut;

    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission();

    // Wait at least 4.5ms
    delay(5);

    // Read two bytes from registers 0xF6 and 0xF7
    ut = readInt(0xF6);
    return ut;
}

void PressureSensor::waitForI2C(uint8_t expectedBytes)
{
    uint32_t startTime = millis();
    
    while(Wire.available() < expectedBytes && startTime - millis() < I2C_TIMEOUT)
        ;

    //Remember that I2C didn't reply last time
    if(startTime - millis() > I2C_TIMEOUT)
    {
        unresponsiveI2C = true;
        Serial.println("I2C seems unresponsive, stopping preassure reading");
    }
}
