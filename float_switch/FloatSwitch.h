#ifndef FLOAT_SWITCH_H
#define FLOAT_SWITCH_H

#include <Arduino.h>

class FloatSwitch {
public:
    // Constructor: pin = GPIO where float switch signal is connected
    // invert = true if the float switch logic is inverted (e.g., LOW means triggered)
    FloatSwitch(uint8_t pin, bool invert = false);

    // Initializes the float switch pin
    void begin();

    // Returns true if the float switch is triggered (water level reached)
    bool isTriggered();

    // Returns the raw HIGH/LOW value from the pin
    int readRaw();

private:
    uint8_t _pin;
    bool _invert;
};

#endif
