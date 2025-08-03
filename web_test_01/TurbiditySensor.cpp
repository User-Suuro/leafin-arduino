#include "TurbiditySensor.h"

TurbiditySensor::TurbiditySensor(int pin) {
  analogPin = pin;
}

float TurbiditySensor::readVoltage() {
  int sensorValue = analogRead(analogPin);
  return sensorValue * (5.0 / 1024.0);
}
