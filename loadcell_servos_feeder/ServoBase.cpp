#include "ServoBase.h"

ServoBase::ServoBase(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay)
    : pin(pin), homeAngle(homeAngle), activeAngle(activeAngle), stepDelay(stepDelay), state(IDLE), currentPos(homeAngle), lastStepTime(0) {}

void ServoBase::update() {
    if (state == IDLE) return;

    unsigned long now = millis();
    if (now - lastStepTime < stepDelay) return;

    lastStepTime = now;

    if (state == MOVING_TO_ACTIVE) {
        if (currentPos < activeAngle) {
            currentPos++;
            servo.write(currentPos);
        } else {
            state = IDLE;
        }
    } else if (state == MOVING_TO_HOME) {
        if (currentPos > homeAngle) {
            currentPos--;
            servo.write(currentPos);
        } else {
            state = IDLE;
        }
    }
}

void ServoBase::startActivate() {
    if (state == IDLE) {
        state = MOVING_TO_ACTIVE;
    }
}

void ServoBase::startDeactivate() {
    if (state == IDLE) {
        state = MOVING_TO_HOME;
    }
}

bool ServoBase::isIdle() const {
    return state == IDLE;
}
