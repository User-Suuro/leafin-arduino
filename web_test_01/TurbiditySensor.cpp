#include "TurbiditySensor.h"

TurbiditySensor::TurbiditySensor(uint8_t pin, float referenceVoltage, int resolution)
  : _pin(pin), _refVoltage(referenceVoltage), _resolution(resolution) {}

float TurbiditySensor::getValue() {
  int sensorValue = analogRead(_pin);
  float voltage = sensorValue * (_refVoltage / _resolution); 
  return voltage;
}
