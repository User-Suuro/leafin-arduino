#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include <Arduino.h>
#include "GravityTDS.h"

class TDSSensor {
public:
    TDSSensor(uint8_t pin, float aref = 5.0, uint16_t adcRange = 1024);

    void begin();
    void update(float temperature);  // Pass temperature directly when updating
    float getValue() const;          // in ppm

private:
    uint8_t _pin;
    float _aref;
    uint16_t _adcRange;
    float _tdsValue;

    GravityTDS gravityTds;
};

#endif
