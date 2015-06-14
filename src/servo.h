#include <Arduino.h>

#include <stdint.h>

#ifndef H_SERVO
#define H_SERVO

class Servo
{
public:
    Servo(int pin);
    
    void setValue(uint8_t pulseWidth);
private:
    uint8_t pin;

    uint8_t pulseWidth;
};

#endif
