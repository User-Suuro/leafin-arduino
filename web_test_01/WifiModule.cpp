#include <Arduino.h>
#include "WiFiModule.h"

extern const char* ssid;
extern const char* password;

void sendCommand(const String& cmd, int delayMs, bool showResponse) {
  Serial1.println(cmd);
  delay(delayMs);
  if (showResponse) {
    while (Serial1.available()) {
      Serial.write(Serial1.read());
    }
  }
}

void connectToWiFi() {
  sendCommand("AT+CWMODE=1");
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 8000);
}
