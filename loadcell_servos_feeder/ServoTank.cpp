#include "ServoTank.h"

ServoTank::ServoTank(uint8_t pin) : ServoBase(pin, 0, 90, 20) {}

void ServoTank::begin() {
    servo.attach(pin);
    servo.write(0);
}
