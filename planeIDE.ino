#include <Servo.h>

#include "config.h"
#include "sketch.h"

#ifndef PRO_MICRO
    #define PPM_INTERRUPT_PIN 2
    #define PPM_INTERRUPT_ID 0
#else
    #define PPM_INTERRUPT_PIN 3
    #define PPM_INTERRUPT_ID 0
#endif

Sketch sketch;

void onPPMChange()
{
    int type = digitalRead(PPM_INTERRUPT_PIN);

    //This is a rising interrupt
    if(type == HIGH)
    {
        sketch.onPPMRise();
    }
    else
    {
        sketch.onPPMFall();
    }
}

void setup()
{
    Serial.begin(115200);
    sketch.setup();
    
    interrupts();
    attachInterrupt(PPM_INTERRUPT_ID, onPPMChange, CHANGE);
}

void loop()
{
    sketch.loop();
}

