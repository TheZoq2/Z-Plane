
#include "config.h"
#include "sketch.h"

#include <EEPROM.h>
#include <Wire.h>

#ifdef TEENSY_LC
    #define PPM_INTERRUPT_PIN 2
    #define PPM_INTERRUPT_ID 2
#else
    #ifndef PRO_MICRO
        #define PPM_INTERRUPT_PIN 2
        #define PPM_INTERRUPT_ID 0
    #else
        #define PPM_INTERRUPT_PIN 3
        #define PPM_INTERRUPT_ID 0
    #endif
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
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(PPM_INTERRUPT_PIN, INPUT);

    Serial.begin(115200);
    sketch.setup();
    
    interrupts();
    //attachInterrupt(PPM_INTERRUPT_ID, onPPMChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PPM_INTERRUPT_PIN), onPPMChange, CHANGE);
    digitalWrite(13, HIGH);
}

void loop()
{
    sketch.loop();
}

