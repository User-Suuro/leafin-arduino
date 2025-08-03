#include "HX711Sensor.h"
#include "WiFiModule.h"
#include "HttpClient.h"
#include <Arduino.h>  // ✅ Needed for Stream, Serial1, etc.

const char* ssid     = "X8b";
const char* password = "12345678";
const char* host     = "yamanote.proxy.rlwy.net";
const int port       = 16955;
const char* statusEndpoint = "/api/device-status";
const char* weightEndpoint = "/api/send-weight";

const uint8_t HX_DT_PIN  = 3;
const uint8_t HX_SCK_PIN = 2;

bool isReady = false;
const unsigned long atInterval = 2000;

HX711Sensor sensor(HX_DT_PIN, HX_SCK_PIN);  // ✅ Declare sensor object

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // ✅ For Mega. Make sure you use the correct board.

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
  sensor.begin();  // ✅ Initialize sensor
  makeHTTPRequest(host, port, statusEndpoint, Serial1);
}

void loop() {
  float weight = sensor.readWeight();  // ✅ Get reading
  sendWeightToServer(host, port, weightEndpoint, Serial1, weight);
  delay(5000);
}
