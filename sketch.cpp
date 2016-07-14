#include "sketch.h"

#include <Wire.h>

// Put this line at the top of your program
#define BMP085_ADDRESS 0x77  // I2C address of BMP085

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
    unsigned char data;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BMP085_ADDRESS, 1);
    while(!Wire.available())
    ;

    return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int16_t bmp085ReadInt(unsigned char address)
{
    unsigned char msb, lsb;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom(BMP085_ADDRESS, 2);
    while(Wire.available()<2)
    ;
    msb = Wire.read();
    lsb = Wire.read();

    //return (uint16_t) msb<<8 | lsb;
    return (int16_t) msb<<8 | lsb;
}

const unsigned char OSS = 3;  // Oversampling Setting

// Calibration values
int16_t ac1 = 0;
int16_t ac2 = 0; 
int16_t ac3 = 0; 
uint16_t ac4 = 0;
uint16_t ac5 = 0;
uint16_t ac6 = 0;
int16_t b1 = 0; 
int16_t b2 = 0;
int16_t mb = 0;
int16_t mc = 0;
int16_t md = 0;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)
float altitude;

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
    ac1 = bmp085ReadInt(0xAA);
    ac2 = bmp085ReadInt(0xAC);
    ac3 = bmp085ReadInt(0xAE);
    ac4 = bmp085ReadInt(0xB0);
    ac5 = bmp085ReadInt(0xB2);
    ac6 = bmp085ReadInt(0xB4);
    b1 = bmp085ReadInt(0xB6);
    b2 = bmp085ReadInt(0xB8);
    mb = bmp085ReadInt(0xBA);
    mc = bmp085ReadInt(0xBC);
    md = bmp085ReadInt(0xBE);
}
// Read the uncompensated temperature value
uint16_t bmp085ReadUT()
{
    unsigned int ut;

    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(0xF4);
    Wire.write(0x2E);
    Wire.endTransmission();

    // Wait at least 4.5ms
    delay(5);

    // Read two bytes from registers 0xF6 and 0xF7
    ut = bmp085ReadInt(0xF6);
    return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
    unsigned char msb, lsb, xlsb;
    unsigned long up = 0;

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

    // Wait for data to become available
    while(Wire.available() < 3)
    ;
    msb = Wire.read();
    lsb = Wire.read();
    xlsb = Wire.read();

    up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

    return up;
}
// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(uint16_t ut)
{
    long x1, x2;

    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    b5 = x1 + x2;

    return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
    long x1, x2, x3, b3, b6, p;
    unsigned long b4, b7;

    b6 = b5 - 4000;
    // Calculate B3
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)(up - b3) * (50000>>OSS));
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


Sketch::Sketch() :
    thrust_manager(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN),
    leftAileron(LEFT_AILERON_PIN, Side::LEFT),
    rightAileron(RIGHT_AILERON_PIN, Side::RIGHT),
    elevator(ELEVATOR_PIN),
    tailRudder(TAIL_RUDDER_PIN),
    dropServo(DROP_SERVO_PIN, 0.2, 0.8, [](float channelValue){
            if(channelValue < 0.25)
            {
                return true;
            }
            return false;})
{
}

void Sketch::onPPMFall()
{
    ppmReader.onPPMFall();
}
void Sketch::onPPMRise()
{
    ppmReader.onPPMRise();
}

void Sketch::setup()
{
    Wire.begin();
    bmp085Calibration();
    pressureSensor.calibrate();
}

void Sketch::loop()
{
    ppmReader.calculateChannels();
    
    thrust_manager.update(ppmReader);

    leftAileron.update(ppmReader);
    rightAileron.update(ppmReader);

    elevator.set_value(ppmReader.getChannelValue(Channel::ELEVATOR));
    tailRudder.set_value(ppmReader.getChannelValue(Channel::YAW));


    dropServo.update(ppmReader.getChannelValue(Channel::AUX2));

    
    handleAltimeter();

    //#define PRINT_SERIAL
    #ifdef PRINT_SERIAL
        for (uint8_t i = 0; i < 8; ++i) {
            Serial.print(i);
            Serial.print(": ");
            Serial.print(ppmReader.getChannelValue(i));
        }
        Serial.println(";");
    #endif
}

void Sketch::handleAltimeter()
{
    const float p0 = 101325;

    float dropChannel = ppmReader.getChannelValue(Channel::AUX2);
    float configChannel = ppmReader.getChannelValue(Channel::AUX3);

    if(altimeterState == BUSY)
    {
        //Check if the inputs are back to normal
        if(dropChannel < 0.3 && std::abs(configChannel - 0.5)< 0.1)
        {
            altimeterState = IDLE;
        }
    }
    else
    {
        if(dropChannel > 0.3)
        {
            altimeterState = BUSY;

            short temperature = bmp085GetTemperature(bmp085ReadUT());
            long pressure = bmp085GetPressure(bmp085ReadUP());

            float altitude = this->pressureSensor.getAltitude();
            int8_t altInt = (int16_t) (altitude - alt0);

            //TODO: Remove
            Serial.println("Bomb drop");
            EEPROM.write(currentAddress, altInt);
            currentAddress++;
        }
        
        if(configChannel < 0.3)
        {
            altimeterState = BUSY;
            Serial.println("Set zero");

            short temperature = bmp085GetTemperature(bmp085ReadUT());
            long pressure = bmp085GetPressure(bmp085ReadUP());

            float altitude = this->pressureSensor.getAltitude();
            alt0 = altitude;

            Serial.println(alt0);
        }
        else if(configChannel > 0.7)
        {
            altimeterState = BUSY;
            
            for (uint8_t i = 0; i < 128; ++i) {
                int val = EEPROM.read(i);

                Serial.println(val);
            }
        }
    }
}
