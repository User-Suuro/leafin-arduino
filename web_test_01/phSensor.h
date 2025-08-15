#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>

class pHSensor {
public:
    pHSensor(uint8_t pin, float voltageAtPH7 = 2.5, float slope = -5.0);
    
    void begin();
    float readPH();
    void setOffset(float offset);

    // New: check if sensor is connected
    bool isConnected(float noiseThreshold = 0.05);

private:
    uint8_t _pin;
    float _voltageAtPH7; // Voltage output at pH 7
    float _slope;        // Conversion slope
    float _offset;       // Calibration offset
};

#endif
