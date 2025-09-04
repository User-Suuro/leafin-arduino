#include "Arduino.h"
#include "MQ137Sensor.h"

#define MQ137_PIN A0 // Analog pin connected to MQ137 output

MQ137Sensor ammonia(MQ137_PIN);

void setup() {
    Serial.begin(9600);
    ammonia.begin();

    // Calibrate: values here are examples; you must adjust based on your experiments
    // Ro is measured in clean air; slope & intercept come from MQ137 datasheet curve
    ammonia.setCalibration(10.0, -0.263, 0.42);

    Serial.println("MQ137 Sensor Initialized");
}

void loop() {
    int raw = ammonia.readRaw();
    float ppm = ammonia.readPPM();

    Serial.print("Raw ADC: ");
    Serial.print(raw);
    Serial.print(" | NH3 PPM: ");
    Serial.println(ppm);

    delay(1000);
}
