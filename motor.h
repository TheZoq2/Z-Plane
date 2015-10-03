#ifndef H_MOTOR
#define H_MOTOR

#include <Arduino.h>
#include <Servo.h>

#include "cppmReader.h"
#include "auxSwitch.h"
#include "config.h"

//At 5 percent throttle, the motor might start
const float MOTOR_START_BOUND = 0.11;

const uint16_t ESC_MIN_VAL = 1050;
const uint16_t ESC_MAX_VAL = 1900;

class Motor
{
public:
    enum State
    {
        STARTED,
        UNARMED,
        ARMED
    };

    Motor();

    void setup(uint8_t pin, uint8_t channel, AuxSwitch* armSwitch);

    void update(CPPMReader* ppmReader);

    uint16_t getEscValue();
    bool getArmState();
private:
    State state; 

    uint8_t pin;
    uint8_t channel;
    uint16_t escValue;

    Servo esc;

    AuxSwitch* armSwitch;
};

#endif
