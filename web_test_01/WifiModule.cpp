#include "WiFiModule.h"
#include <EEPROM.h>

bool isWiFiConnected(Stream& espSerial) {
  // Clear any previous response
  while (espSerial.available()) espSerial.read();

  espSerial.println("AT+CIPSTATUS");
  unsigned long start = millis();
  String response = "";

  while (millis() - start < 2000) {
    while (espSerial.available()) {
      response += char(espSerial.read());
    }
  }

  return response.indexOf("STATUS:2") >= 0 || response.indexOf("STATUS:3") >= 0;
}

void connectToWiFi(const char* ssid, const char* password, Stream& espSerial, Stream& logSerial) {
  if (isWiFiConnected(espSerial)) {
    logSerial.println("📶 Already connected to WiFi");
    return;
  }

  logSerial.println("🔌 Connecting to WiFi...");

  // Clear buffer
  while (espSerial.available()) espSerial.read();

  espSerial.println("AT+CWMODE=1");
  delay(2000);

  String cmd = String("AT+CWJAP=\"") + ssid + "\",\"" + password + "\"";
  espSerial.println(cmd);

  unsigned long start = millis();
  String response = "";

  while (millis() - start < 10000) { // 10 second timeout
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }

  logSerial.println("📡 WiFi Join Response:");
  logSerial.println(response);

  if (response.indexOf("WIFI CONNECTED") >= 0 || response.indexOf("OK") >= 0) {
    logSerial.println("✅ WiFi Connected!");
  } else {
    logSerial.println("❌ Failed to connect.");
  }
}

