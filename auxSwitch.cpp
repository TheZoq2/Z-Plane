#include "auxSwitch.h"

void AuxSwitch::setup(uint8_t channel, float lowBounds, float highBounds)
{
    this->channel = channel;

    this->bounds.low = lowBounds;
    this->bounds.high = highBounds;
}

void AuxSwitch::update(CPPMReader* ppmReader)
{
    this->value = ppmReader->getChannelValue(channel);

    if(this->value > bounds.low && this->value < bounds.high)
    {
        inBounds = true;
    }
    else
    {
        inBounds = false;
    }
}

float AuxSwitch::getValue()
{
    return this->value;
}

bool AuxSwitch::isInBounds()
{
    return inBounds;
}

