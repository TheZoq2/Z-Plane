#include "cppmReader.h"

const float CPPMReader::LOW_VALID_VALUE = -0.1;
const float CPPMReader::HIGH_VALID_VALUE = 1.1;

CPPMReader::CPPMReader()
{
}

void CPPMReader::onPPMFall()
{
    uint16_t pulseLength = micros() - currentPulseStart;
    
    cppmPulses[cIndex] = pulseLength;

    cIndex++;

    if(cIndex >= PULSE_AMOUNT)
    {
        digitalWrite(12, HIGH);

        cIndex = 0;
    }
}

void CPPMReader::onPPMRise()
{
    currentPulseStart = micros();
}

void CPPMReader::calculateChannels()
{
    //Finding the sync pulse
    uint8_t syncID = 0;
    uint16_t longestPulse = 0;
    for(uint8_t i = 0; i < PULSE_AMOUNT; i++)
    {
        if(cppmPulses[i] > longestPulse)
        {
            longestPulse = cppmPulses[i];
            syncID = i;
        }
    }

    valuesAreValid = true;
    //Calculating the channel values
    for(uint8_t i = 0; i < CHANNEL_AMOUNT; i++)
    {
        uint8_t arrayIndex = syncID + i + 1;

        if(arrayIndex >= PULSE_AMOUNT)
            arrayIndex = arrayIndex - PULSE_AMOUNT;

        //Calculating the channel value
        int16_t pulseDiff = cppmPulses[arrayIndex] - MIN_PULSE;
        float channelValue = pulseDiff / (float) PULSE_LENGTH;
        
        if(channelValue < CPPMReader::LOW_VALID_VALUE || channelValue > CPPMReader::HIGH_VALID_VALUE)
        {
            channels[i] = 0.5;
            valuesAreValid = false;
        }
        else
        {
            channels[i] = channelValue;
        }
    }

}

float CPPMReader::getChannelValue(uint8_t channelID)
{
    return channels[channelID];
}

uint8_t CPPMReader::getChannelAmount()
{
    return CHANNEL_AMOUNT;
}
uint16_t CPPMReader::getPulseValue(uint8_t index)
{
    return cppmPulses[index];
}
bool CPPMReader::getValuesAreValid()
{
    return valuesAreValid;
}

