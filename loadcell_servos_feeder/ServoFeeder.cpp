#include "ServoFeeder.h"

ServoFeeder::ServoFeeder(uint8_t pin) : ServoBase(pin, 60, 180, 15) {}

void ServoFeeder::begin() {
    servo.attach(pin);
    servo.write(60);
}
