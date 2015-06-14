#include <Arduino.h>
#include <Servo.h>

#include <stdint.h>

#include "cppmReader.h"

#ifndef H_SKETCH
#define H_SKETCH


class Sketch
{
public:
    void onPPMFall();
    void onPPMRise();

    void setup();
    void loop();
private:
    CPPMReader ppmReader;

    int oldServoPos;
    Servo testServo;
};

#endif
