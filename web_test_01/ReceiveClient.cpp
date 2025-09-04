#include "ReceiveClient.h"

String receiveCommandFromWeb(
  const char* host,
  int port,
  const char* endpoint,
  Stream& espSerial,
  Stream& logSerial
) {
    logSerial.println("🌐 Receiving command from server...");
    while (espSerial.available()) espSerial.read();

    // -- TCP connect --
    String connectCmd = String("AT+CIPSTART=\"TCP\",\"") + host + "\"," + port;
    espSerial.println(connectCmd);

    unsigned long start = millis();
    String connectResp = "";
    while (millis() - start < 5000) {
        while (espSerial.available()) {
            connectResp += (char)espSerial.read();
        }
        if (connectResp.indexOf("OK") >= 0 || connectResp.indexOf("ALREADY CONNECT") >= 0) break;
    }

    if (connectResp.indexOf("ERROR") >= 0 || connectResp.indexOf("FAIL") >= 0) {
        logSerial.println("❌ TCP connection failed:");
        logSerial.println(connectResp);
        return "";
    }

    // -- HTTP GET request --
    String request = "GET " + String(endpoint) + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n";

    espSerial.print("AT+CIPSEND=");
    espSerial.println(request.length());

    // wait for ">" prompt
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
        return "";
    }

    // send request
    espSerial.print(request);

    // -- read full response --
    String response = "";
    start = millis();
    while (millis() - start < 5000) {
        while (espSerial.available()) {
            response += (char)espSerial.read();
        }
        if (response.indexOf("CLOSED") != -1) break;
    }

    logSerial.println("✅ Raw server response:");
    logSerial.println(response);

    // -- extract JSON payload --
    int jsonStart = response.indexOf("{");
    int jsonEnd = response.lastIndexOf("}");
    if (jsonStart == -1 || jsonEnd == -1) return "";

    String jsonPayload = response.substring(jsonStart, jsonEnd + 1);

    // -- close connection --
    espSerial.println("AT+CIPCLOSE");
    delay(200);

    return jsonPayload;   // return whole JSON string
}
