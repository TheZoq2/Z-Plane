#include "AWServo.h"
#include <Arduino.h>

AWServo::AWServo(uint8_t pin)
{
    analogWriteFrequency(pin, FREQUENCY);
    analogWriteResolution(RESOLUTION);

    this->pin = pin;
}

void AWServo::write(float value)
{
    if(value > maxCmd)
    {
        value = maxCmd;
    }
    else if(value < minCmd)
    {
        value = minCmd;
    }

    uint16_t write_value = MIN_VAL + (MAX_VAL - MIN_VAL) * value;

    analogWrite(pin, write_value);
}

void AWServo::setCmdLimits(float min, float max)
{
    this->minCmd = min;
    this->maxCmd = max;
}
