//Servo implementation using analog write
#ifndef H_AW_SERVO
#define H_AW_SERVO

#include <cinttypes>

class AWServo 
{
public:
    AWServo(uint8_t pin);

    void write(float value);
private:
    static const uint8_t RESOLUTION = 16;
    static const uint8_t FREQUENCY = 50;

    static const uint16_t MIN_VAL = 1800;
    static const uint16_t MAX_VAL = 7900;

    uint8_t pin;
};
#endif
