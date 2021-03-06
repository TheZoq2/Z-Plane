#ifndef H_AILERON
#define H_AILERON

#include <cinttypes>

#include "AWServo.h"
#include "cppmReader.h"

enum Side
{
    LEFT,
    RIGHT,
};

class Aileron 
{
public:
    Aileron(uint8_t pin, Side side);

    void update(CPPMReader &ppm_reader);
private:
    AWServo servo;
    
    uint8_t mainChannel;
    uint8_t flapChannel;
    Side side;
};
#endif
