#ifndef H_RUDDER
#define H_RUDDER

#include <cinttypes>

#include "AWServo.h"

class Rudder 
{
public:
    Rudder(uint8_t pin);

    void set_value(float val);
private:
    AWServo servo;
};
#endif
