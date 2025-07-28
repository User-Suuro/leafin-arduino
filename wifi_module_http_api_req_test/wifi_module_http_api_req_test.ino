#include <SoftwareSerial.h>

SoftwareSerial espSerial(3, 2); // RX, TX

bool isReady = false;
unsigned long lastAT = 0;
const unsigned long atInterval = 2000;

// === Configuration ===
const char* ssid     = "X8b";
const char* password = "12345678";
const char* host     = "jsonplaceholder.typicode.com";
const int port       = 80; // HTTP
const char* endpoint = "/posts/1";

void sendCommand(String cmd, int delayMs = 2000) {
  espSerial.println(cmd);
  delay(delayMs);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
}

void connectToWiFi() {
  sendCommand("AT+CWMODE=1");
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 5000);
}

void makeHTTPRequest() {
  sendCommand("AT+CIPMUX=0");
  sendCommand("AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(port), 5000);

  String httpRequest =
    "GET " + String(endpoint) + " HTTP/1.1\r\n" +
    "Host: " + String(host) + "\r\n" +
    "Connection: close\r\n\r\n";

  sendCommand("AT+CIPSEND=" + String(httpRequest.length()));
  delay(1000);
  espSerial.print(httpRequest);

  // Read response
  unsigned long timeout = millis();
  while (millis() - timeout < 10000) {
    if (espSerial.available()) {
      Serial.write(espSerial.read());
      timeout = millis(); // Reset timeout on activity
    }
  }
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  delay(3000);

  // Check AT communication
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
