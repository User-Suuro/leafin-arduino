#include <Arduino.h>
#include "TDS_Sensor.h"

#define TDSSensorPin A3

TDSSensor tds(TDSSensorPin);

// Example: using a variable for temperature
float waterTemp = 25.0; // Replace with actual sensor reading

void setup() {
    Serial.begin(9600);
    tds.begin();
}

void loop() {
    // Example: If you have a DS18B20, update waterTemp here
    // waterTemp = readDS18B20();

    tds.update(waterTemp);
    Serial.print(tds.getValue(), 0);
    Serial.println("ppm");

    delay(1000);
}
