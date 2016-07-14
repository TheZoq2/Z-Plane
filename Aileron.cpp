#include "Aileron.h"

Aileron::Aileron(uint8_t pin, Side side):
    servo(pin)
{
    this->side = side;

    servo.setCmdLimits(-0.2, 1.2);
}

void Aileron::update(CPPMReader &ppmReader)
{
    float flapVal = 0;
    
    if(ppmReader.getChannelValue(Channel::AUX1) < .5)
    {
        flapVal = 0.3;

        if(side == Side::RIGHT)
        {
            flapVal = -0.3;
        }
    }


    float val = 1-ppmReader.getChannelValue(Channel::AILERON) + flapVal;

    //TODO: Allow scaling and expo and that kind of fancy stuff
    servo.write(val);
}
