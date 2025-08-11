#include <Arduino.h>
#include "LoadCellSensor.h"
#include "ServoFeeder.h"
#include "ServoTank.h"

const uint8_t HX711_DOUT_PIN = 4;
const uint8_t HX711_SCK_PIN = 5;
const float CALIBRATION_FACTOR = 1419.29;

const uint8_t SERVO_FEEDER_PIN = 6;
const uint8_t SERVO_TANK_PIN = 7;

const float WEIGHT_THRESHOLD = 15.0;

LoadCellSensor loadCell(HX711_DOUT_PIN, HX711_SCK_PIN, CALIBRATION_FACTOR);
ServoFeeder feederServo(SERVO_FEEDER_PIN);
ServoTank tankServo(SERVO_TANK_PIN);

enum SystemState {
    TANK_OPENING,
    WAITING_WEIGHT,
    TANK_CLOSING,
    FEEDER_SWEEPING,
    IDLE
};

SystemState systemState = TANK_OPENING;
bool hasTared = false;
unsigned long stateStartTime = 0;

void setup() {
    Serial.begin(57600);
    delay(10);

    if (!loadCell.begin(2000, true)) {
        Serial.println("Load cell init timeout");
    }

    feederServo.begin();
    tankServo.begin();

    tankServo.startActivate(); // open tank at start
    systemState = TANK_OPENING;
    stateStartTime = millis();
}

void loop() {
    loadCell.update();
    feederServo.update();
    tankServo.update();

    float currentWeight = loadCell.getValue();

    switch (systemState) {
        case TANK_OPENING:
            if (tankServo.isIdle()) {
                Serial.println("Tank opened");
                systemState = WAITING_WEIGHT;
            }
            break;

        case WAITING_WEIGHT:
            if (currentWeight >= WEIGHT_THRESHOLD) {
                Serial.println("Weight threshold reached");
                tankServo.startDeactivate(); // close tank
                systemState = TANK_CLOSING;
            }
            break;

        case TANK_CLOSING:
            if (tankServo.isIdle()) {
                Serial.println("Tank closed");
                feederServo.startActivate(); // sweep feeder
                systemState = FEEDER_SWEEPING;
            }
            break;

        case FEEDER_SWEEPING:
            if (feederServo.isIdle()) {
                Serial.println("Feeder sweep complete");
                systemState = IDLE;
            }
            break;

        case IDLE:
            // Idle, waiting for tare command or system reset
            if (Serial.available()) {
                char c = Serial.read();
                if (c == 't' && !hasTared) {
                    loadCell.tare();
                    hasTared = true;
                    Serial.println("Tare command received");
                }
            }
            if (loadCell.isTareComplete() && hasTared) {
                Serial.println("Tare complete");
                hasTared = false;
                systemState = TANK_OPENING;
                tankServo.startActivate(); // open tank again for next cycle
            }
            break;
    }
}
