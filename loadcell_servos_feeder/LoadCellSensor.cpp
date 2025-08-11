#include "LoadCellSensor.h"

LoadCellSensor::LoadCellSensor(uint8_t doutPin, uint8_t sckPin, float calFactor)
    : loadCell(doutPin, sckPin), calibrationFactor(calFactor), tareTimeoutFlag(false) {}

bool LoadCellSensor::begin(unsigned long stabilizingTime, bool doTare) {
    loadCell.begin();
    loadCell.start(stabilizingTime, doTare);
    tareTimeoutFlag = loadCell.getTareTimeoutFlag();
    if (!tareTimeoutFlag) {
        loadCell.setCalFactor(calibrationFactor);
    }
    return !tareTimeoutFlag;
}

bool LoadCellSensor::update() {
    return loadCell.update();
}

float LoadCellSensor::getValue() const {
    return loadCell.getData();
}

void LoadCellSensor::tare() {
    loadCell.tareNoDelay();
}

bool LoadCellSensor::isTareComplete() const {
    return loadCell.getTareStatus();
}
