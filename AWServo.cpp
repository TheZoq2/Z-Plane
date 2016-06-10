#include "AWServo.h"
#include <Arduino.h>

AWServo::AWServo(uint8_t pin)
{
    analogWriteFrequency(pin, FREQUENCY);
    analogWriteResolution(RESOLUTION);

    this->pin = pin;
}

void AWServo::write(float value)
{
    if(value > 1.0f)
    {
        value = 1.0f;
    }
    else if(value < 0.0f)
    {
        value = 0.0f;
    }

    uint16_t write_value = MIN_VAL + (MAX_VAL - MIN_VAL) * value;

    analogWrite(pin, write_value);

    Serial.println(write_value);
}
