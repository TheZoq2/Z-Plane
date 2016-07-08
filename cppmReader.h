#include <Arduino.h>
#include <stdint.h>

#include "config.h"

#ifndef H_CPPMREADER
#define H_CPPMREADER

//The time in miliseconds that is the max and minimum values of the pwm signal
#define MAX_PULSE 1000
#define MIN_PULSE 600
#define PULSE_LENGTH 1000;

enum Channel
{
    THROTTLE = 0,
    YAW = 3,
    AILERON = 1,
    ELEVATOR = 2,
    ARM = 5,
    AUX1 = 4,
};

class CPPMReader
{
public:
    CPPMReader();

    void onPPMRise();
    void onPPMFall();

    void calculateChannels();

    static uint8_t getChannelAmount();
    float getChannelValue(uint8_t channelID);
    float getChannelValue(Channel channel);
    uint16_t getPulseValue(uint8_t index);

    bool getValuesAreValid();
private:
    static const uint8_t CHANNEL_AMOUNT = 8;
    static const uint8_t PULSE_AMOUNT = 9;

    volatile uint16_t cppmPulses[PULSE_AMOUNT];
    volatile uint8_t cIndex;
    volatile uint32_t currentPulseStart;

    float channels[CHANNEL_AMOUNT];

    bool valuesAreValid;

    static const float LOW_VALID_VALUE;
    static const float HIGH_VALID_VALUE;
};

#endif
