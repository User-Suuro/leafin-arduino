#include "AmmoniaEstimator.h"
#include <math.h>

AmmoniaEstimator::AmmoniaEstimator(MQ137Sensor& mq, pHSensor& ph, DS18B20Sensor& temp)
    : _mq(mq), _ph(ph), _temp(temp), _nh3GasPPM(0), _pHValue(7.0), _waterTempC(25.0) {}

void AmmoniaEstimator::begin() {
    _mq.begin();
    _ph.begin();
    _temp.begin();
}

void AmmoniaEstimator::update() {
    _nh3GasPPM = _mq.readPPM();
    _pHValue = _ph.readPH();
    _waterTempC = _temp.readTemperature();
}

float AmmoniaEstimator::getNH3GasPPM() {
    return _nh3GasPPM;
}

float AmmoniaEstimator::getFractionNH3() {
    float tempK = _waterTempC + 273.15;
    float pKa = 0.09018 + (2729.92 / tempK);
    float fraction = 1.0 / (1.0 + pow(10, (pKa - _pHValue)));
    return fraction;
}

float AmmoniaEstimator::getTotalAmmoniaPPM() {
    float fraction = getFractionNH3();
    if (fraction > 0) {
        return _nh3GasPPM / fraction;
    }
    return 0;
}
