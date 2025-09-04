#include <Arduino.h>
#include "LoadCellSensor.h"
#include "ServoFeeder.h"
#include "ServoTank.h"

// HX711 load cell pins and calibration
const uint8_t HX711_DOUT_PIN = 2;
const uint8_t HX711_SCK_PIN = 3;
const float CALIBRATION_FACTOR = 1419.29;

// Servo pins
const uint8_t SERVO_FEEDER_PIN = 6;
const uint8_t SERVO_TANK_PIN = 7;

// Weight threshold to trigger action
const float WEIGHT_THRESHOLD = 15.0;

// Objects
ServoFeeder feederServo(SERVO_FEEDER_PIN, 60, 180, 15); // home=60°, active=180°, step=15ms
ServoTank tankServo(SERVO_TANK_PIN, 0, 90, 20);         // home=0°, active=90°, step=20ms
LoadCellSensor loadCell(HX711_DOUT_PIN, HX711_SCK_PIN, CALIBRATION_FACTOR);

// System states
enum SystemState {
    FEEDER_OPENING,    // Open feeder first
    WAITING_WEIGHT,    // Wait for weight threshold
    FEEDER_CLOSING,    // Close feeder
    TANK_SWEEPING,     // Sweep feed with tank
    IDLE
};

SystemState systemState = FEEDER_OPENING;
bool hasTared = false;

void setup() {
    Serial.begin(9600);
    delay(10);

    // Initialize load cell
    if (!loadCell.begin(2000, true)) {
        Serial.println("Load cell init timeout");
    }

    // Initialize servos
    feederServo.begin();
    tankServo.begin();

    // Start by opening feeder
    feederServo.startActivate();
}

void loop() {
    // Update load cell and servos
    loadCell.update();
    feederServo.update();
    tankServo.update();

    float currentWeight = loadCell.getValue();
    Serial.print("HX711 Weight: ");
    Serial.println(currentWeight);

    switch (systemState) {
        case FEEDER_OPENING:
            if (feederServo.isIdle()) {
                Serial.println("Feeder opened");
                systemState = WAITING_WEIGHT;
            }
            break;

        case WAITING_WEIGHT:
            if (currentWeight >= WEIGHT_THRESHOLD) {
                Serial.println("Weight threshold reached");
                feederServo.startDeactivate();  // close feeder
                systemState = FEEDER_CLOSING;
            }
            break;

        case FEEDER_CLOSING:
            if (feederServo.isIdle()) {
                Serial.println("Feeder closed");
                tankServo.startActivate();      // sweep feed
                systemState = TANK_SWEEPING;
            }
            break;

        case TANK_SWEEPING:
            if (tankServo.isIdle()) {
                Serial.println("Tank sweep complete");
                systemState = IDLE;
            }
            break;

        case IDLE:
            // Handle tare command via Serial
            if (Serial.available()) {
                char c = Serial.read();
                if (c == 't' && !hasTared) {
                    loadCell.tare();
                    hasTared = true;
                    Serial.println("Tare command received");
                }
            }

            if (hasTared && loadCell.isTareComplete()) {
                Serial.println("Tare complete");
                hasTared = false;
                // Restart cycle
                feederServo.startActivate();
                systemState = FEEDER_OPENING;
            }
            break;
    }
}
