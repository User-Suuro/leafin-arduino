#ifndef LOAD_CELL_SENSOR_H
#define LOAD_CELL_SENSOR_H

#include <Arduino.h>
#include <HX711_ADC.h>

class LoadCellSensor {
public:
    // Constructor: takes data pin, clock pin, and calibration factor
    LoadCellSensor(uint8_t doutPin, uint8_t sckPin, float calibrationFactor);

    // Initialize sensor, returns true if successful
    bool begin(unsigned long stabilizingTime = 2000, bool doTare = true);

    // Call repeatedly to update data; returns true if new data available
    bool update();

    // Get the current weight reading
    float getValue() const;

    // Trigger tare operation without delay
    void tare();

    // Check if tare operation is complete
    bool isTareComplete() const;

private:
    HX711_ADC loadCell;
    float calibrationFactor;
    bool tareTimeoutFlag;
};

#endif // LOAD_CELL_SENSOR_H
