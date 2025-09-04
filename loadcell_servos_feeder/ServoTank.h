#ifndef SERVO_TANK_H
#define SERVO_TANK_H

#include <Arduino.h>
#include <Servo.h>

class ServoTank {
public:
    ServoTank(uint8_t pin, int homeAngle, int activeAngle, unsigned int stepDelay);

    void begin();                 // Attach servo and move to home
    void startActivate();         // Open tank
    void startDeactivate();       // Close tank
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
