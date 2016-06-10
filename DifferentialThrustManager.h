#ifndef H_DIFFERENTIAL_THRUST_MANAGER
#define H_DIFFERENTIAL_THRUST_MANAGER

#include <cinttypes>
#include "AWServo.h"

class DifferentialThrustManager 
{
public:
    DifferentialThrustManager(uint8_t left_pin, uint8_t right_pin);
    
    void update(float throttle, float rudder);
private:
    static constexpr float THROTTLE_SCALE = 1.0f;
    static constexpr float RUDDER_SCALE = 0.15f;

    uint8_t left_pin;
    uint8_t right_pin;

    AWServo left_servo;
    AWServo right_servo;
};
#endif
