#include "motor.h"

Motor::Motor()
{
    this->state = STARTED;
}

void Motor::setup(uint8_t pin, uint8_t channel, AuxSwitch* armSwitch)
{
    this->channel = channel;
    this->pin = pin;

    this->armSwitch = armSwitch;

    this->esc.attach(pin);
}

void Motor::update(CPPMReader* ppmReader)
{
    armSwitch->update(ppmReader);

    //Serial.println(armSwitch->getValue());

    if(state == STARTED)
    {
        this->escValue = ESC_MIN_VAL;

        //In order for the motor to arm, both the throttle and arm stick must first be in an unarmed mode
        if(armSwitch->isInBounds() == false && ppmReader->getChannelValue(channel) < MOTOR_START_BOUND && ppmReader->getValuesAreValid() == true)
        {
            state = UNARMED;
        }
    }
    else if(state == UNARMED)
    {
        if(armSwitch->isInBounds() == true && ppmReader->getChannelValue(channel) < MOTOR_START_BOUND)
        {
            state = ARMED;
        }

        escValue = ESC_MIN_VAL;
    }
    else if(state == ARMED)
    {
        if(armSwitch->isInBounds() == false)
        {
            state = UNARMED;
        }

        escValue = ESC_MIN_VAL + (ESC_MAX_VAL - ESC_MIN_VAL) * ppmReader->getChannelValue(channel);
    }

    this->esc.writeMicroseconds(escValue);
}

uint16_t Motor::getEscValue()
{
    return this->escValue;
}
bool Motor::getArmState()
{
    return this->state;
}
