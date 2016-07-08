#include "sketch.h"


Sketch::Sketch() :
    thrust_manager(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN),
    left_aileron(LEFT_AILERON_PIN, 1, FLAP_CHANNEL, Side::LEFT),
    right_aileron(RIGHT_AILERON_PIN, 1, FLAP_CHANNEL, Side::RIGHT),
    elevator(ELEVATOR_PIN)
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

    //left_aileron.set_value(1 - ppmReader.getChannelValue(1));
    //right_aileron.set_value(1 - ppmReader.getChannelValue(1));
    left_aileron.update(ppmReader);
    right_aileron.update(ppmReader);

    elevator.set_value(ppmReader.getChannelValue(PITCH_CHANNEL));
    
    //#define PRINT_SERIAL
    #ifdef PRINT_SERIAL
        Serial.print("0: ");
        Serial.print(ppmReader.getChannelValue(0));
        Serial.print("  1: ");
        Serial.print(ppmReader.getChannelValue(1));
        Serial.print("  2: ");
        Serial.print(ppmReader.getChannelValue(2));
        Serial.print("  3: ");
        Serial.println(ppmReader.getChannelValue(3));
    #endif
}

