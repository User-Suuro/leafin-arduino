#include "Arduino.h"
#include "DS18B20Sensor.h"

// Use the Arduino pin where your DS18B20's data wire is connected
// For pluggable terminal wiring: 
//  - GND to sensor GND
//  - VCC to 5V or 3.3V (check module spec)
//  - Data to selected pin, with a 4.7kΩ pull-up resistor to VCC
#define DS18B20_PIN 9

DS18B20Sensor waterTemp(DS18B20_PIN);

void setup() {
    Serial.begin(9600);
    waterTemp.begin();

    if (waterTemp.isConnected()) {
        Serial.println("DS18B20 sensor connected successfully!");
    } else {
        Serial.println("No DS18B20 sensor detected!");
    }
}

void loop() {
    float tempC = waterTemp.readCelsius();
    float tempF = waterTemp.readFahrenheit();

    Serial.print("Water Temperature: ");
    Serial.print(tempC);
    Serial.print(" °C  /  ");
    Serial.print(tempF);
    Serial.println(" °F");

    delay(1000);
}
