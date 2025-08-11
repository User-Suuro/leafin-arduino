#ifndef SERVO_FEEDER_H
#define SERVO_FEEDER_H

#include "ServoBase.h"

class ServoFeeder : public ServoBase {
public:
    ServoFeeder(uint8_t pin);

    void begin();
};

#endif // SERVO_FEEDER_H
