#include "TurbiditySensor.h"

TurbiditySensor::TurbiditySensor(int pin) {
  analogPin = pin;
}

float TurbiditySensor::readVoltage() {
  int sensorValue = analogRead(analogPin);
  return sensorValue * (5.0 / 1023.0); // Corrected to 1023.0
}

float TurbiditySensor::readNTU() {
  float voltage = readVoltage();

  // Example calibration curve (adjust according to your sensor's datasheet)
  // This formula is common for SEN0189-type turbidity sensors
  float ntu;
  if (voltage < 2.5) {
    ntu = 3000; // Max measurable turbidity
  } else {
    ntu = -1120.4 * sq(voltage) + 5742.3 * voltage - 4352.9;
  }
  return ntu;
}
