#include "elevon.h"

void Elevon::setup(uint16_t minVal, uint16_t maxVal, Side side, uint8_t servoPin, uint8_t rollChannel, uint8_t pitchChannel)
{
    this->minVal = minVal;
    this->maxVal = maxVal;

    this->side = side;

    //this->servo = servo;
    this->servo.attach(servoPin);

    this->rollChannel = rollChannel;
    this->pitchChannel = pitchChannel;

    this->aileronTrim = 0;
    this->elevatorTrim = 0;
}

void Elevon::update(CPPMReader* ppmReader)
{
    float aileron = 1 - (1.1 * ppmReader->getChannelValue(rollChannel));
    float elevator = 1 - (0.9 * ppmReader->getChannelValue(pitchChannel));

    //IF this elevon is on the left side, it needs to be inverted
    if(side == LEFT)
    {
        aileron = 1 - aileron;
    }


    float finalValue = (elevator / 2 + aileron / 2);

    if(reversed)
    {
        finalValue = 1 - finalValue;
    }

    servoValue = minVal +  (maxVal - minVal) * finalValue;

    servo.writeMicroseconds(servoValue);
}

uint16_t Elevon::getCurrentValue()
{
    return servoValue;
}

void Elevon::setReversed(bool reversed)
{
    this->reversed = reversed;
}

/*
void Elevon::setElevatorTrim(uint8_t elevatorTrim)
{
    this->elevatorTrim = elevatorTrim;
}
void Elevon::setAileronTrim(uint16_t aileronTrim)
{
    this->aileronTrim = aileronTrim;
}
*/
