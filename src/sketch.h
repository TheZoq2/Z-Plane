#include <Arduino.h>
#include <stdint.h>

#include "cppmReader.h"
#include "servo.h"

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

    Servo* testServo;
};

#endif
