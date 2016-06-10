#include "DifferentialThrustManager.h"
#include <Arduino.h>

DifferentialThrustManager::DifferentialThrustManager(uint8_t left_pin, uint8_t right_pin) :
    left_servo(left_pin),
    right_servo(right_pin)
{
}

void DifferentialThrustManager::update(float throttle, float rudder)
{
    float left_value = throttle * THROTTLE_SCALE - (rudder - 0.5) * RUDDER_SCALE;

    if(left_value > 1.0)
    {
        left_value = 1.0;
    }
    else if(left_value < 0.0)
    {
        left_value = 0;
    }

    float right_value = throttle * THROTTLE_SCALE + (rudder - 0.5) * RUDDER_SCALE;

    if(right_value > 1.0)
    {
        right_value = 1.0;
    }
    else if(right_value < 0.0)
    {
        right_value = 0;
    }

    left_servo.write(left_value);
    right_servo.write(right_value);
}
