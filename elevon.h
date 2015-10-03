#include <Servo.h>
#include <Arduino.h>

#include <stdint.h>

#include "cppmReader.h"

#include "config.h"

#ifndef H_ELEVON
#define H_ELEVON


class Elevon
{
public:
    enum Side
    {
        LEFT,
        RIGHT
    };

    void setup(uint16_t minVal, uint16_t maxVal, Side side, uint8_t servoPin, uint8_t pitchChannel, uint8_t rollChannel);

    void update(CPPMReader* ppmReader);

    uint16_t getCurrentValue();

    void setElevatorTrim(uint8_t elevatorTrim);
    void setAileronTrim(uint8_t aileronTrim);
    void setReversed(bool reversed);
private:
    Side side;
    uint16_t minVal;
    uint16_t maxVal;

    uint16_t aileronTrim;
    uint16_t elevatorTrim;

    uint8_t pitchChannel;
    uint8_t rollChannel;

    Servo servo;
    uint16_t servoValue;

    bool reversed;
};

#endif
