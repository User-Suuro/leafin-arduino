#include "FloatSwitch.h"

FloatSwitch::FloatSwitch(uint8_t pin, bool invert)
    : _pin(pin), _invert(invert) {}

void FloatSwitch::begin() {
    pinMode(_pin, INPUT_PULLUP); 
    // Most float switches use normally open/closed mechanical contacts.
    // Using INPUT_PULLUP avoids needing an external resistor.
}

bool FloatSwitch::isTriggered() {
    int state = digitalRead(_pin);
    return _invert ? (state == LOW) : (state == HIGH);
}

int FloatSwitch::readRaw() {
    return digitalRead(_pin);
}
