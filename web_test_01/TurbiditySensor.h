#ifndef TURBIDITY_SENSOR_H
#define TURBIDITY_SENSOR_H

#include <Arduino.h>

class TurbiditySensor {
  private:
    int analogPin;

  public:
    TurbiditySensor(int pin);
    float readVoltage();
    float readNTU();
    bool isConnected();
};

#endif
