#ifndef SERVO_FEEDER_H
#define SERVO_FEEDER_H

#include <Arduino.h>
#include <Servo.h>

class ServoFeeder {
public:
    ServoFeeder(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay);

    void begin();                 // Attach servo and move to home
    void startActivate();         // Sweep to active angle
    void startDeactivate();       // Return to home
    void update();                // Call in loop to move servo gradually
    bool isIdle() const;          // True if movement finished

private:
    Servo servo;
    uint8_t pin;
    int homeAngle;
    int activeAngle;
    int targetAngle;
    int currentAngle;
    unsigned int stepDelay;       // milliseconds per step
    unsigned long lastMoveTime;
};

#endif
