#ifndef TURBIDITY_SENSOR_H
#define TURBIDITY_SENSOR_H

#include <Arduino.h>

class TurbiditySensor {
  public:
    TurbiditySensor(uint8_t pin, float referenceVoltage = 5.0, int resolution = 1024);
    float getValue();

  private:
    uint8_t _pin;
    float _refVoltage;
    int _resolution;
};

#endif
