#ifndef H_DIFFERENTIAL_THRUST_MANAGER
#define H_DIFFERENTIAL_THRUST_MANAGER

#include <cinttypes>
#include "AWServo.h"
#include "cppmReader.h"

class DifferentialThrustManager 
{
public:
    DifferentialThrustManager(uint8_t left_pin, uint8_t right_pin);
    
    void update(CPPMReader& ppmReader);
private:
    static constexpr float THROTTLE_SCALE = 1.0f;
    static constexpr float RUDDER_SCALE = 0.30f;
    static constexpr float MIN_THROTTLE_COMMAND = 0.12f;

    uint8_t left_pin;
    uint8_t right_pin;

    AWServo left_servo;
    AWServo right_servo;

    uint8_t throttleChannel = 0;
    uint8_t rudderChannel = 3;
    uint8_t armChannel = 5;

    bool isArmed;
};
#endif
