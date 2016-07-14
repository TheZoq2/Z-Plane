#ifndef H_BOOL_SERVO
#define H_BOOL_SERVO

#include <stdint.h>

#include "AWServo.h"

class BoolServo 
{
public:
    BoolServo(uint8_t pin, float trueVal, float falseVal, bool (*func)(float channelValue));

    void update(float channelValue);
private:
    float trueVal;
    float falseVal;
    bool (*func)(float channelvalue);

    AWServo servo;
};
#endif
