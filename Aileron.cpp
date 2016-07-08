#include "Aileron.h"

Aileron::Aileron(uint8_t pin, uint8_t mainChannel, uint8_t flapChannel, Side side):
    rudder(pin)
{
    this->mainChannel = mainChannel;
    this->flapChannel = flapChannel;
    this->side = side;
}

void Aileron::update(CPPMReader &ppmReader)
{
    float flapVal = 0;
    
    if(ppmReader.getChannelValue(flapChannel) < .5)
    {
        flapVal = 0.3;

        if(side == Side::RIGHT)
        {
            flapVal = -0.3;
        }
    }


    float val = 1-ppmReader.getChannelValue(mainChannel) + flapVal;

    //TODO: Allow scaling and expo and that kind of fancy stuff
    rudder.set_value(val);
}
