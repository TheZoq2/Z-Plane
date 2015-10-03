#include "config.h"

#ifndef H_AUX_SWITCH
#define H_AUX_SWITCH

#include "stdint.h"
#include "cppmReader.h"

class AuxSwitch
{
public:
    struct Bounds
    {
        float low;
        float high;
    };
    
    void setup(uint8_t channel, float lowBound, float highBound);
    void update(CPPMReader* ppmReader);

    float getValue();
    bool isInBounds();
private:
    uint8_t channel;

    float value;
    bool inBounds;

    Bounds bounds;
};

#endif
