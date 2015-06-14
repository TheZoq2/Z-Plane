#include "servo.h"

Servo::Servo(int pin)
{
    this->pin = pin;

    pinMode(pin, OUTPUT);

    setValue(128);
}

void Servo::setValue(uint8_t pulseWidth)
{
    //if(this->pulseWidth != pulseWidth)
    //{
        this->pulseWidth = pulseWidth;

        analogWrite(pin, pulseWidth);
    //}
}


