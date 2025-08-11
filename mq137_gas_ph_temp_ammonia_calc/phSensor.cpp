#include "pHSensor.h"

pHSensor::pHSensor(uint8_t pin) : _pin(pin), _offset(0.0) {}

void pHSensor::begin() {
    pinMode(_pin, INPUT);
}

float pHSensor::readPH() {
    int raw = analogRead(_pin);
    float voltage = (float)raw * 5.0 / 1023.0;
    // Example: typical pH board outputs 0-14 pH as ~0-5V
    float phValue = 3.5 * voltage + _offset; // adjust 3.5 and offset by calibration
    return phValue;
}
