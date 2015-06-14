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
    testServo = new Servo(10);
    
    pinMode(10, OUTPUT);
    analogWrite(10, 128);
}

void Sketch::loop()
{
    //ppmReader.calculateChannels();
    testServo->setValue((ppmReader.getChannelValue(0) * 255));
    //
    Serial.println((ppmReader.getChannelValue(0) * 255));
}
