#include "pHSensor.h"

pHSensor::pHSensor(uint8_t pin, float voltageAtPH7, float slope)
    : _pin(pin), _voltageAtPH7(voltageAtPH7), _slope(slope), _offset(0.0) {}

void pHSensor::begin() {
    pinMode(_pin, INPUT);
}

float pHSensor::readPH() {
    int raw = analogRead(_pin);
    float voltage = (float)raw * (5.0 / 1023.0);

    // Calculate pH based on calibration
    float phValue = 7.0 + ((voltage - _voltageAtPH7) * _slope) + _offset;

    return phValue;
}

void pHSensor::setOffset(float offset) {
    _offset = offset;
}

bool pHSensor::isConnected(float noiseThreshold) {
    const int samples = 10;
    float sum = 0.0;

    for (int i = 0; i < samples; i++) {
        sum += analogRead(_pin);
        delay(5);
    }

    float avgRaw = sum / samples;
    float voltage = avgRaw * 5.0 / 1023.0;

    // Consider the sensor connected if voltage is not just noise (0V or near 5V)
    return (voltage > noiseThreshold && voltage < (5.0 - noiseThreshold));
}
