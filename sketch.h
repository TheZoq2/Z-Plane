#include <Arduino.h>

#include <stdint.h>

#include "cppmReader.h"
#include "auxSwitch.h"
#include "config.h"
#include "AWServo.h"
#include "DifferentialThrustManager.h"
#include "Rudder.h"
#include "Aileron.h"

#ifndef H_SKETCH
#define H_SKETCH

class Sketch
{
public:
    Sketch();

    void onPPMFall();
    void onPPMRise();

    void setup();
    void loop();
private:
    CPPMReader ppmReader;

    DifferentialThrustManager thrust_manager;

    Aileron left_aileron;
    Aileron right_aileron;
    Rudder elevator;

    #ifdef TEENSY_LC
        static const uint8_t LEFT_MOTOR_PIN = 23;
        static const uint8_t RIGHT_MOTOR_PIN = 22;

        static const uint8_t LEFT_AILERON_PIN = 16;
        static const uint8_t RIGHT_AILERON_PIN = 17;

        static const uint8_t BACK_RUDDER_PIN = 10;

        static const uint8_t ELEVATOR_PIN = 9;
    #endif
};

#endif
