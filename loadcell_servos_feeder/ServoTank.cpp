#include "ServoTank.h"

ServoTank::ServoTank(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay)
    : pin(pin), homeAngle(homeAngle), activeAngle(activeAngle), stepDelay(stepDelay),
      currentAngle(homeAngle), targetAngle(homeAngle), lastMoveTime(0) {}

void ServoTank::begin() {
    servo.attach(pin);
    servo.write(homeAngle);
    currentAngle = homeAngle;
    targetAngle = homeAngle;
}

void ServoTank::startActivate() {
    targetAngle = activeAngle;
}

void ServoTank::startDeactivate() {
    targetAngle = homeAngle;
}

void ServoTank::update() {
    if (millis() - lastMoveTime < stepDelay) return;
    lastMoveTime = millis();

    if (currentAngle < targetAngle) currentAngle++;
    else if (currentAngle > targetAngle) currentAngle--;

    servo.write(currentAngle);
}

bool ServoTank::isIdle() const {
    return currentAngle == targetAngle;
}
