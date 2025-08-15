#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class DS18B20Sensor {
public:
    // Constructor: pin = GPIO where data line of DS18B20 is connected
    DS18B20Sensor(uint8_t pin);

    // Initializes the sensor
    void begin();

    // Reads the current temperature in Celsius
    float readCelsius();

    // Reads the current temperature in Fahrenheit
    float readFahrenheit();

    // Returns true if a valid reading was obtained
    bool isConnected();

private:
    uint8_t _pin;
    OneWire _oneWire;
    DallasTemperature _sensors;
};

#endif
