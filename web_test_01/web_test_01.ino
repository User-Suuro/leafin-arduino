#include <Arduino.h>
#include "WiFiModule.h"
#include "HttpClient.h"
#include "TurbiditySensor.h"

const char* ssid     = "X8b";
const char* password = "12345678";
const char* host     = "yamanote.proxy.rlwy.net";
const int port       = 16955;

const char* sendStatusEndpoint    = "/api/device-status";
const char* sendWeightEndpoint    = "/api/send-weight";
const char* sendTurbidityEndpoint = "/api/send-turbidity";

bool isReady = false;
const unsigned long atInterval = 2000;

// Turbidity sensor on A0
TurbiditySensor turbiditySensor(A0);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  delay(3000);
  while (!isReady) {
    Serial1.println("AT");
    delay(atInterval);
    if (Serial1.find("OK")) {
      Serial.println("✅ ESP-01S Ready");
      isReady = true;
    } else {
      Serial.println("⌛ Waiting for ESP...");
    }
  }

  connectToWiFi(ssid, password, Serial1);
  delay(5000);
}

void loop() {
  sendConnectionStatus(host, port, sendStatusEndpoint, Serial1);
  delay(5000);

  float turbidityValue = turbiditySensor.readVoltage();
  sendTurbidityToServer(host, port, sendTurbidityEndpoint, Serial1, turbidityValue);
  delay(5000);
}
