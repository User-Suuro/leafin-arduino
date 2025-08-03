#include "phSensor.h"
#include <Arduino.h>

PhSensor::PhSensor(int analogPin) {
  _analogPin = analogPin;
}

float PhSensor::readPH() {
  int analogValue = analogRead(_analogPin);
  float voltage = analogValue * (5.0 / 1023.0);
  float pH = 7.0 + ((voltage - voltageAtPH7) * slope);
  
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);

  return pH;
}
