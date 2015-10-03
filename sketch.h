#include <Arduino.h>
#include <Servo.h>

#include <stdint.h>

#include "cppmReader.h"
#include "elevon.h"
#include "motor.h"
#include "auxSwitch.h"
#include "config.h"

#ifndef H_SKETCH
#define H_SKETCH

const uint8_t MOTOR_PIN = 8;
const uint8_t MOTOR_CHANNEL = 0;

const uint8_t ARM_CHANNEL = 5;
const float ARM_LOW_BOUND = 0.25;
const float ARM_HIGH_BOUND = 0.6; //A bit of padding added

const uint8_t ROLL_CHANNEL = 1;
const uint8_t PITCH_CHANNEL = 2;

const uint16_t ELEVON_MIN = 1200;
const uint16_t ELEVON_MAX = 1800;

class Sketch
{
public:
    void onPPMFall();
    void onPPMRise();

    void setup();
    void loop();
private:
    CPPMReader ppmReader;

    Elevon leftElevon;
    Elevon rightElevon;

    Motor motor;
    
    AuxSwitch armSwitch;
};

#endif