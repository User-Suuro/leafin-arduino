#include "HX711Sensor.h"

HX711Sensor::HX711Sensor(uint8_t dataPin, uint8_t clockPin)
  : _dataPin(dataPin), _clockPin(clockPin), scale(dataPin, clockPin) {}

void HX711Sensor::begin() {
  scale.begin();
  scale.start(2000);      // Wait for stabilization (optional)
  scale.setCalFactor(1.0); // Placeholder, should be set to your calibration factor
  scale.tare();           // Reset scale to 0
}

float HX711Sensor::readWeight() {
  if (scale.update()) {
    return scale.getData();
  }
  return 0.0; // Or some error indicator
}
