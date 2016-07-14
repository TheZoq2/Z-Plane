//Servo implementation using analog write
#ifndef H_AW_SERVO
#define H_AW_SERVO

#include <cinttypes>

class AWServo 
{
public:
    AWServo(uint8_t pin);

    void write(float value);

    /*
     * Sets the minimum input value to the servo as an f32. Undefined things will
     * happen if you set it below and above some value so don't set it too high or low.
     */
    void setCmdLimits(float min, float max);
private:
    static const uint8_t RESOLUTION = 16;
    static const uint8_t FREQUENCY = 50;

    //static const uint16_t MIN_VAL = 1800;
    //static const uint16_t MAX_VAL = 7900;
    static const uint16_t MAX_VAL = 6553;
    static const uint16_t MIN_VAL = 3225;

    uint8_t pin;

    float minCmd = 0.0f;
    float maxCmd = 1.0f;
};
#endif
