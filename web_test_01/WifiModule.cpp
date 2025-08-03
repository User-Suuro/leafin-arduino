#include "WiFiModule.h"

void sendCommand(const String& cmd, Stream& espSerial, int delayMs, bool showResponse) {
  espSerial.println(cmd);
  delay(delayMs);
  if (showResponse) {
    while (espSerial.available()) {
      Serial.write(espSerial.read());
    }
  }
}

void connectToWiFi(const char* ssid, const char* password, Stream& espSerial) {
  sendCommand("AT+CWMODE=1", espSerial, 2000);
  String joinCmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
  sendCommand(joinCmd, espSerial, 8000);
}
