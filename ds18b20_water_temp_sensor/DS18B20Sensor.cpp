#include "DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(uint8_t pin)
    : _pin(pin), _oneWire(pin), _sensors(&_oneWire) {}

void DS18B20Sensor::begin() {
    _sensors.begin();
}

float DS18B20Sensor::readCelsius() {
    _sensors.requestTemperatures();
    return _sensors.getTempCByIndex(0);
}

float DS18B20Sensor::readFahrenheit() {
    _sensors.requestTemperatures();
    return _sensors.getTempFByIndex(0);
}

bool DS18B20Sensor::isConnected() {
    DeviceAddress tempDeviceAddress;
    return _sensors.getAddress(tempDeviceAddress, 0);
}
