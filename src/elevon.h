#include <Servo.h>
#include <Arduino.h>

#include <stdint.h>

#ifndef H_ELEVON
#define H_ELEVON


class Elevon
{
public:
    enum Direction
    {
        LEFT,
        RIGHT
    };

    void setup(uint16_t minVal, uint16_t maxVal, Direction direction);

    void update(int elevator, int aileron);

    void printCurrent();
private:
    Direction direction;
    uint16_t minVal;
    uint16_t maxVal;
};

#endif
