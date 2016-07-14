#include "BoolServo.h"

BoolServo::BoolServo(uint8_t pin, float trueVal, float falseVal, 
        bool (*func)(float channelValue)):
    servo(pin)
{
    this->trueVal = trueVal;
    this->falseVal = falseVal;
    this->func = func;
}

void BoolServo::update(float channelValue)
{
    if(func(channelValue))
    {
        servo.write(trueVal);
    }
    else
    {
        servo.write(falseVal);
    }
}
