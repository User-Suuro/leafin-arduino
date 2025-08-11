#ifndef PH_SENSOR_H
#define PH_SENSOR_H

#include <Arduino.h>

class pHSensor {
public:
    pHSensor(uint8_t pin);
    void begin();
    float readPH();

private:
    uint8_t _pin;
    float _offset;
};

#endif
