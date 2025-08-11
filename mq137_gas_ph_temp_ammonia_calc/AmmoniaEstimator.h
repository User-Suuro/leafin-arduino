#ifndef AMMONIA_ESTIMATOR_H
#define AMMONIA_ESTIMATOR_H

#include <Arduino.h>
#include "MQ137Sensor.h"   // From our earlier MQ-137 code
#include "pHSensor.h"      // We'll define this for pH
#include "DS18B20Sensor.h" // From earlier DS18B20 water temp code

class AmmoniaEstimator {
public:
    AmmoniaEstimator(MQ137Sensor& mq, pHSensor& ph, DS18B20Sensor& temp);

    void begin();
    void update();
    float getNH3GasPPM();
    float getFractionNH3();
    float getTotalAmmoniaPPM();

private:
    MQ137Sensor& _mq;
    pHSensor& _ph;
    DS18B20Sensor& _temp;

    float _nh3GasPPM;
    float _pHValue;
    float _waterTempC;
};

#endif
