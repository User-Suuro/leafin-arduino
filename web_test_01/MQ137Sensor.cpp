#include "MQ137Sensor.h"

MQ137Sensor::MQ137Sensor(uint8_t pin)
    : _pin(pin), _Ro(10.0), _slope(-0.263), _intercept(0.42) {
    // Default Ro, slope, intercept values can be adjusted later with setCalibration()
}

void MQ137Sensor::begin() {
    pinMode(_pin, INPUT);
}

int MQ137Sensor::readRaw() {
    return analogRead(_pin);
}

float MQ137Sensor::readPPM() {
    int rawADC = readRaw();
    float Rs = calculateResistance(rawADC);
    float ratio = Rs / _Ro; // Rs/Ro
    // Using log-log equation: log10(PPM) = slope * log10(Rs/Ro) + intercept
    float ppm = pow(10, (_slope * log10(ratio) + _intercept));
    return ppm;
}

void MQ137Sensor::setCalibration(float Ro, float slope, float intercept) {
    _Ro = Ro;
    _slope = slope;
    _intercept = intercept;
}

float MQ137Sensor::calculateResistance(int rawADC) {
    // Assuming a load resistor of 10kΩ
    const float RL = 10.0;
    float voltage = (float)rawADC * 5.0 / 1023.0; // for 5V ADC reference
    return (5.0 - voltage) * RL / voltage;
}
