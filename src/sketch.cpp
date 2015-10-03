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
    testServo.attach(10);

    //testServo.writeMicroseconds(1500);
}

void Sketch::loop()
{

    ppmReader.calculateChannels();
    int newServoPos = 1000 + 1000 * ppmReader.getChannelValue(2);

    testServo.writeMicroseconds(newServoPos);
    Serial.println(newServoPos);
}

