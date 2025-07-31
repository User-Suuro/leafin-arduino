#include <SoftwareSerial.h>

SoftwareSerial espSerial(3, 2); // RX, TX

// === Flags and Timers ===
bool isReady = false;
unsigned long lastAT = 0;
const unsigned long atInterval = 2000;

// === Configuration ===
const char* ssid     = "X8b";
const char* password = "12345678";
const char* host     = "yamanote.proxy.rlwy.net";  // ✅ Correct: no port in domain
const int port       = 16955;                      // ✅ Connect to correct port
const char* endpoint = "/api/device-status";

void sendCommand(String cmd, int delayMs = 2000, bool showResponse = true) {
  espSerial.println(cmd);
  delay(delayMs);
  if (showResponse) {
    while (espSerial.available()) {
      Serial.write(espSerial.read());
    }
  }
}

void connectToWiFi() {
  sendCommand("AT+CWMODE=1");
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 8000);
}

void makeHTTPRequest() {
  sendCommand("AT+CIPMUX=0");
  sendCommand("AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(port), 5000);

  String httpRequest =
    "GET " + String(endpoint) + " HTTP/1.1\r\n" +
    "Host: " + String(host) + ":" + String(port) + "\r\n" +  // ✅ Include port in header only
    "Connection: close\r\n\r\n";

  sendCommand("AT+CIPSEND=" + String(httpRequest.length()), 2000);
  delay(1000);
  espSerial.print(httpRequest); // Correct: do not use println

  // Read response
  unsigned long timeout = millis();
  bool found = false;

  Serial.println("\n[Response Start]");
  while (millis() - timeout < 10000) {
    while (espSerial.available()) {
      char c = espSerial.read();
      Serial.write(c);
      timeout = millis(); // Reset timeout on activity
      if (c == '{') found = true;
    }
  }
  Serial.println("\n[Response End]");

  if (found) {
    Serial.println("✅ ESP received valid response");
  } else {
    Serial.println("❌ ESP failed to read expected response");
  }

}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  delay(3000);

  while (!isReady) {
    espSerial.println("AT");
    delay(atInterval);
    if (espSerial.find("OK")) {
      Serial.println("ESP-01S Ready");
      isReady = true;
    } else {
      Serial.println("Waiting for ESP...");
    }
  }

  connectToWiFi();
  makeHTTPRequest();
}

void loop() {
  // Nothing to do in loop
}
