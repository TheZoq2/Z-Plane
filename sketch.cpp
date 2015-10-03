#include "sketch.h"

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
    leftElevon.setup(ELEVON_MIN, ELEVON_MAX, Elevon::LEFT, 10, PITCH_CHANNEL, ROLL_CHANNEL);
    rightElevon.setup(ELEVON_MIN, ELEVON_MAX, Elevon::RIGHT, 9, PITCH_CHANNEL, ROLL_CHANNEL);

    leftElevon.setReversed(false);

    armSwitch.setup(ARM_CHANNEL, ARM_LOW_BOUND, ARM_HIGH_BOUND);

    motor.setup(MOTOR_PIN, MOTOR_CHANNEL, &armSwitch);
}

void Sketch::loop()
{
    ppmReader.calculateChannels();
    //int newServoPos = 1000 + 1000 * ppmReader.getChannelValue(2);

    leftElevon.update(&ppmReader);
    rightElevon.update(&ppmReader);
    motor.update(&ppmReader);
    
    #ifdef PRINT_SERIAL
        Serial.print("Left: ");
        Serial.print(leftElevon.getCurrentValue());
        Serial.print("  Right: ");
        Serial.print(rightElevon.getCurrentValue());
        Serial.print("  Motor: ");
        Serial.print(motor.getEscValue());
        Serial.print("  Armed: ");
        Serial.println(motor.getArmState());
    #endif
}

