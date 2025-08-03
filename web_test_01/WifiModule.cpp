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
  sendCommand(joinCmd, espSerial, 8000, true);  // show initial join response

  // Wait for Wi-Fi connection confirmation
  bool connected = false;
  unsigned long startTime = millis();
  const unsigned long timeout = 15000; // 15 seconds timeout

  Serial.print("⏳ Connecting to Wi-Fi");

  while (millis() - startTime < timeout) {
    sendCommand("AT+CWJAP?", espSerial, 1000, false); // Check connection status

    String response = "";
    unsigned long responseStart = millis();
    while (millis() - responseStart < 1000 && espSerial.available()) {
      response += char(espSerial.read());
    }

    if (response.indexOf(ssid) >= 0) {
      connected = true;
      break;
    }

    Serial.print(".");
    delay(500);
  }

  Serial.println(); // for newline

  if (connected) {
    Serial.println("✅ Wi-Fi connected.");
  } else {
    Serial.println("❌ Failed to connect to Wi-Fi.");
    while (true);  // Stop here forever until reset
  }
}
