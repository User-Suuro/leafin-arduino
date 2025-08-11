#include "HttpClient.h"

// Use Sensor Data Builder, to build the jsonPayLoad

void sendAllSensorDataToServer(
  const char* host,
  int port,
  const char* endpoint,
  Stream& espSerial,
  const String& jsonPayload,
  Stream& logSerial
) {
  logSerial.println("🌐 Sending sensor data...");
  while (espSerial.available()) espSerial.read();

  String connectCmd = String("AT+CIPSTART=\"TCP\",\"") + host + "\"," + port;
  espSerial.println(connectCmd);
  delay(2000);

  String connectResp = "";
  unsigned long start = millis();
  while (millis() - start < 3000) {
    while (espSerial.available()) {
      connectResp += char(espSerial.read());
    }
  }

  if (connectResp.indexOf("ERROR") >= 0 || connectResp.indexOf("FAIL") >= 0) {
    logSerial.println("❌ TCP Connection Failed:");
    logSerial.println(connectResp);
    return;
  }

  // Construct HTTP request
  String request =
    "POST " + String(endpoint) + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Content-Type: application/json\r\n" +
    "Content-Length: " + String(jsonPayload.length()) + "\r\n" +
    "Connection: close\r\n\r\n" +
    jsonPayload;

  espSerial.print("AT+CIPSEND=");
  espSerial.println(request.length());

  bool promptReceived = false;
  start = millis();
  while (millis() - start < 5000) {
    if (espSerial.find(">")) {
      promptReceived = true;
      break;
    }
  }

  if (!promptReceived) {
    logSerial.println("❌ Timeout waiting for > prompt");
    return;
  }

  espSerial.print(request);

  String serverResp = "";
  start = millis();
  while (millis() - start < 5000) {
    while (espSerial.available()) {
      serverResp += char(espSerial.read());
    }
  }

  logSerial.println("✅ Server Response:");
  logSerial.println(serverResp);

  espSerial.println("AT+CIPCLOSE");
}
