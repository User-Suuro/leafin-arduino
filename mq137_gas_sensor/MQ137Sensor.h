#ifndef MQ137_SENSOR_H
#define MQ137_SENSOR_H

#include <Arduino.h>

class MQ137Sensor {
public:
    // Constructor: pin = analog pin connected to MQ137 output
    MQ137Sensor(uint8_t pin);

    // Initialize sensor
    void begin();

    // Read raw analog value (0–1023 on most Arduinos)
    int readRaw();

    // Read NH3 concentration in PPM (requires calibration)
    float readPPM();

    // Set calibration parameters (Ro = sensor resistance in clean air, slope & intercept from datasheet/experiment)
    void setCalibration(float Ro, float slope, float intercept);

private:
    uint8_t _pin;
    float _Ro;        // Sensor resistance in clean air
    float _slope;     // Slope of log curve
    float _intercept; // Intercept of log curve

    float calculateResistance(int rawADC);
};

#endif
