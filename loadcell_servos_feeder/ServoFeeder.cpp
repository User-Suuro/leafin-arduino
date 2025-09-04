#include "ServoFeeder.h"

ServoFeeder::ServoFeeder(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay)
    : pin(pin), homeAngle(homeAngle), activeAngle(activeAngle), stepDelay(stepDelay),
      currentAngle(homeAngle), targetAngle(homeAngle), lastMoveTime(0) {}

void ServoFeeder::begin() {
    servo.attach(pin);
    servo.write(homeAngle);
    currentAngle = homeAngle;
    targetAngle = homeAngle;
}

void ServoFeeder::startActivate() {
    targetAngle = activeAngle;
}

void ServoFeeder::startDeactivate() {
    targetAngle = homeAngle;
}

void ServoFeeder::update() {
    if (millis() - lastMoveTime < stepDelay) return;
    lastMoveTime = millis();

    if (currentAngle < targetAngle) currentAngle++;
    else if (currentAngle > targetAngle) currentAngle--;

    servo.write(currentAngle);
}

bool ServoFeeder::isIdle() const {
    return currentAngle == targetAngle;
}
