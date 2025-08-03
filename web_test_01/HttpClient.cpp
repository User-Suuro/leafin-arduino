#include "HttpClient.h"

void flushESP(Stream& espSerial) {
  while (espSerial.available()) espSerial.read();
}

bool waitForResponse(Stream& espSerial, const char* keyword, unsigned long timeout = 5000) {
  unsigned long start = millis();
  String response = "";
  while (millis() - start < timeout) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
      Serial.write(c); // Print ESP response to Serial monitor for debugging
      if (response.indexOf(keyword) != -1) {
        return true;
      }
    }
  }
  return false;
}

void sendConnectionStatus(const char* host, int port, const char* endpoint, Stream& espSerial) {
  flushESP(espSerial);

  espSerial.print("AT+CIPSTART=\"TCP\",\"");
  espSerial.print(host);
  espSerial.print("\",");
  espSerial.println(port);
  
  if (!waitForResponse(espSerial, "CONNECT")) {
    Serial.println("❌ Wifi Module Failed to connect to server.");
    return;
  }

  String httpRequest = String("GET ") + endpoint + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n";

  espSerial.print("AT+CIPSEND=");
  espSerial.println(httpRequest.length());
  if (!waitForResponse(espSerial, ">")) {
    Serial.println("❌ ESP8266 did not prompt for data.");
    return;
  }

  espSerial.print(httpRequest);
  waitForResponse(espSerial, "SEND OK");
  espSerial.println("AT+CIPCLOSE");
}

void sendWeightToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float weight) {
  flushESP(espSerial);

  espSerial.print("AT+CIPSTART=\"TCP\",\"");
  espSerial.print(host);
  espSerial.print("\",");
  espSerial.println(port);
  if (!waitForResponse(espSerial, "CONNECT")) {
    Serial.println("❌ HX711 Failed to connect to server.");
    return;
  }

  String body = String("{\"weight\":") + weight + "}";
  String request = String("POST ") + endpoint + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + body.length() + "\r\n\r\n" +
                   body;

  espSerial.print("AT+CIPSEND=");
  espSerial.println(request.length());
  if (!waitForResponse(espSerial, ">")) {
    Serial.println("❌ ESP8266 did not prompt for data.");
    return;
  }

  espSerial.print(request);
  waitForResponse(espSerial, "SEND OK");
  espSerial.println("AT+CIPCLOSE");
}

void sendTurbidityToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float voltage) {
  flushESP(espSerial);

  // Start TCP connection
  espSerial.print("AT+CIPSTART=\"TCP\",\"");
  espSerial.print(host);
  espSerial.print("\",");
  espSerial.println(port);

  // Wait for plain-text response from ESP
  if (!waitForResponse(espSerial, "OK") && !waitForResponse(espSerial, "CONNECT")) {
    Serial.println("❌ Turbidity: Failed to connect to server.");
    return;
  }

  // Construct JSON body
  String body = String("{\"turbidity\":") + voltage + "}";
  String request = String("POST ") + endpoint + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + body.length() + "\r\n\r\n" +
                   body;

  // Send request length
  espSerial.print("AT+CIPSEND=");
  espSerial.println(request.length());

  if (!waitForResponse(espSerial, ">")) {
    Serial.println("❌ ESP8266 did not prompt for data.");
    return;
  }

  // Send the request body
  espSerial.print(request);

  // Wait for confirmation that data was sent
  waitForResponse(espSerial, "SEND OK");

  // Close connection
  espSerial.println("AT+CIPCLOSE");

  Serial.println("✅ Turbidity data sent successfully.");
}


