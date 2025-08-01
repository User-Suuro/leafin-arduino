#ifndef HX711SENSOR_H
#define HX711SENSOR_H

#include <Arduino.h>
#include <HX711_ADC.h>

class HX711Sensor {
  public:
    HX711Sensor(uint8_t dataPin, uint8_t clockPin);
    void begin();
    float readWeight();

  private:
    uint8_t _dataPin;
    uint8_t _clockPin;
    HX711_ADC scale;
};

#endif
