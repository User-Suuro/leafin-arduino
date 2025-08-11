#ifndef SERVO_BASE_H
#define SERVO_BASE_H

#include <Arduino.h>
#include <Servo.h>

class ServoBase {
public:
    ServoBase(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay);

    // Call repeatedly in loop() to update servo position
    void update();

    // Start sweeping from home -> active position
    void startActivate();

    // Start sweeping back from active -> home position
    void startDeactivate();

    // Check if servo is idle (not moving)
    bool isIdle() const;

private:
    Servo servo;
    uint8_t pin;
    int homeAngle;
    int activeAngle;
    unsigned int stepDelay;

    enum State {IDLE, MOVING_TO_ACTIVE, MOVING_TO_HOME} state = IDLE;
    int currentPos;
    unsigned long lastStepTime;

    void moveTowards(int targetAngle);
};

#endif // SERVO_BASE_H
