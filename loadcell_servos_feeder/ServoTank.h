#ifndef SERVO_TANK_H
#define SERVO_TANK_H

#include "ServoBase.h"

class ServoTank : public ServoBase {
public:
    ServoTank(uint8_t pin);

    void begin();
};

#endif // SERVO_TANK_H
