#include "sketch.h"


Sketch::Sketch() :
    thrust_manager(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN),
    leftAileron(LEFT_AILERON_PIN, Side::LEFT),
    rightAileron(RIGHT_AILERON_PIN, Side::RIGHT),
    elevator(ELEVATOR_PIN),
    tailRudder(TAIL_RUDDER_PIN),
    dropServo(DROP_SERVO_PIN, 0.0, 0.7, [](float channelValue){
            if(channelValue < 0.25)
            {
                return true;
            }
            return false;})
{
}

void Sketch::onPPMFall()
{
    ppmReader.onPPMFall();
}
void Sketch::onPPMRise()
{
    ppmReader.onPPMRise();
}

void Sketch::setup()
{
}

void Sketch::loop()
{
    ppmReader.calculateChannels();
    //int newServoPos = 1000 + 1000 * ppmReader.getChannelValue(2);

    //leftElevon.update(&ppmReader);
    //rightElevon.update(&ppmReader);
    //motor.update(&ppmReader);
    
    thrust_manager.update(ppmReader);

    //leftAileron.set_value(1 - ppmReader.getChannelValue(1));
    //rightAileron.set_value(1 - ppmReader.getChannelValue(1));
    leftAileron.update(ppmReader);
    rightAileron.update(ppmReader);

    elevator.set_value(ppmReader.getChannelValue(Channel::ELEVATOR));
    tailRudder.set_value(ppmReader.getChannelValue(Channel::YAW));


    dropServo.update(ppmReader.getChannelValue(Channel::AUX2));

    
    handleAltimeter();

    //#define PRINT_SERIAL
    #ifdef PRINT_SERIAL
        for (uint8_t i = 0; i < 8; ++i) {
            Serial.print(i);
            Serial.print(": ");
            Serial.print(ppmReader.getChannelValue(i));
        }
        Serial.println(";");
    #endif
}

void Sketch::handleAltimeter()
{
    float dropChannel = ppmReader.getChannelValue(Channel::AUX2);
    float configChannel = ppmReader.getChannelValue(Channel::AUX3);

    if(altimeterState == BUSY)
    {
        //Check if the inputs are back to normal
        if(dropChannel < 0.3 && std::abs(configChannel) - 0.5 < 0.1)
        {
            altimeterState = IDLE;
        }
    }
    else
    {
        if(dropChannel > 0.3)
        {
            altimeterState = BUSY;

            //TODO: Remove
            Serial.println("Bomb drop");
        }
        
        if(configChannel < 0.3)
        {
            altimeterState = BUSY;
            Serial.println("Set zero");
        }
        else if(configChannel > 0.7)
        {
            altimeterState = BUSY;
            Serial.println("memdump");
        }
    }
}
