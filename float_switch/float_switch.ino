#include <FloatSwitch.h>

// Pluggable terminal wiring:
//  - One wire to GND
//  - Other wire to selected Arduino pin
// The built-in pull-up will hold the pin HIGH until the switch closes.
#define FLOAT_SWITCH_PIN 3

// If your float switch closes when water is HIGH, use invert = true
FloatSwitch waterLevel(FLOAT_SWITCH_PIN, true);

void setup() {
    Serial.begin(9600);
    waterLevel.begin();
    Serial.println("Float Switch Initialized");
}

void loop() {
    if (waterLevel.isTriggered()) {
        Serial.println("⚠ Water level reached!");
    } else {
        Serial.println("Water level normal.");
    }

    delay(500);
}
