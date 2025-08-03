#ifndef PH_SENSOR_H
#define PH_SENSOR_H

class PhSensor {
  public:
    PhSensor(int analogPin);
    float readPH();

  private:
    int _analogPin;
    const float voltageAtPH7 = 2.5; // Default voltage at pH 7
    const float slope = -5.0;       // Default slope of sensor
};

#endif
