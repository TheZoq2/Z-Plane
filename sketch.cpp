#include "sketch.h"


Sketch::Sketch() :
    thrust_manager(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN),
    left_aileron(LEFT_AILERON_PIN),
    right_aileron(RIGHT_AILERON_PIN),
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
    //leftElevon.setup(ELEVON_MIN, ELEVON_MAX, Elevon::LEFT, 10, PITCH_CHANNEL, ROLL_CHANNEL);
    //rightElevon.setup(ELEVON_MIN, ELEVON_MAX, Elevon::RIGHT, 9, PITCH_CHANNEL, ROLL_CHANNEL);

    //leftElevon.setReversed(false);

    armSwitch.setup(ARM_CHANNEL, ARM_LOW_BOUND, ARM_HIGH_BOUND);

    //motor.setup(MOTOR_PIN, MOTOR_CHANNEL, &armSwitch);
}

void Sketch::loop()
{
    ppmReader.calculateChannels();
    //int newServoPos = 1000 + 1000 * ppmReader.getChannelValue(2);

    //leftElevon.update(&ppmReader);
    //rightElevon.update(&ppmReader);
    //motor.update(&ppmReader);
    
    thrust_manager.update(ppmReader.getChannelValue(0), ppmReader.getChannelValue(3));

    left_aileron.set_value(1 - ppmReader.getChannelValue(1));
    right_aileron.set_value(1 - ppmReader.getChannelValue(1));

    elevator.set_value(ppmReader.getChannelValue(2));
    
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

