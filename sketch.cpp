#include "sketch.h"

#include <Wire.h>

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
    pinMode(13, OUTPUT);

    Wire.begin();
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
