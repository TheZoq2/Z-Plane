#include "Rudder.h"

Rudder::Rudder(uint8_t pin):
    servo(pin)
{
}

void Rudder::set_value(float val)
{
    //TODO: Allow scaling and expo and that kind of fancy stuff
    servo.write(val);
}
