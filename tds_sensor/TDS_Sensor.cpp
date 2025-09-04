#include "TDS_Sensor.h"

TDSSensor::TDSSensor(uint8_t pin, float aref, uint16_t adcRange)
    : _pin(pin), _aref(aref), _adcRange(adcRange),
      _tdsValue(0.0) {
}

void TDSSensor::begin() {
    gravityTds.setPin(_pin);
    gravityTds.setAref(_aref);
    gravityTds.setAdcRange(_adcRange);
    gravityTds.begin();
}

void TDSSensor::update(float temperature) {
    gravityTds.setTemperature(temperature);
    gravityTds.update();
    _tdsValue = gravityTds.getTdsValue();
}

float TDSSensor::getValue() const {
    return _tdsValue;
}
