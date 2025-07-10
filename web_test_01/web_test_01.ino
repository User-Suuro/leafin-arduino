#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

String incomingBuffer = "";

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  Serial.println("Connecting...");
  sendCommand("AT", 1000, true);
  sendCommand("AT+CWMODE=1", 1000, true);
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 8000, true);

  // MQTT Configuration
  sendCommand("AT+MQTTUSERCFG=0,1,\"esp01client\",\"\",\"\",0,0,\"\"", 1000, true);

  // Connect to MQTT Broker
  sendCommand("AT+MQTTCONN=0,\"broker.hivemq.com\",1883,0", 5000, true);

  // Subscribe to a topic to receive commands
  sendCommand("AT+MQTTSUB=0,\"device/commands\",1", 1000, true);

  Serial.println("Setup complete. Waiting for MQTT messages...");
}

void loop() {
  // Listen for incoming MQTT messages
  while (espSerial.available()) {
    char c = espSerial.read();
    incomingBuffer += c;
    Serial.print(c);  // Optional: log everything

    // Handle complete line
    if (c == '\n') {
      processIncomingLine(incomingBuffer);
      incomingBuffer = "";  // reset after processing
    }
  }

  // You can send data periodically if needed
  // sendData("{\"temp\":25,\"status\":\"ok\"}");
}

void sendCommand(String cmd, int timeout, bool debug) {
  espSerial.println(cmd);
  long int time = millis();
  while ((millis() - time) < timeout) {
    while (espSerial.available()) {
      char c = espSerial.read();
      if (debug) Serial.write(c);
    }
  }
}

void sendData(String json) {
  sendCommand("AT+MQTTPUB=0,\"esp/data\",\"" + json + "\",1,0", 2000, true);
}

void processIncomingLine(String line) {
  // Check for MQTT subscription message
  if (line.startsWith("+MQTTSUBRECV")) {
    int payloadStart = line.indexOf("\",\""); // locate payload start
    if (payloadStart != -1) {
      String payload = line.substring(payloadStart + 3, line.length() - 2); // extract payload
      Serial.println("\n[MQTT RECEIVED] Payload: " + payload);

      // Example: parse basic command
      if (payload == "LED_ON") {
        Serial.println("[ACTION] Turn LED ON");
        // digitalWrite(LED_BUILTIN, LOW); // active-low for many boards
      } else if (payload == "LED_OFF") {
        Serial.println("[ACTION] Turn LED OFF");
        // digitalWrite(LED_BUILTIN, HIGH);
      } else {
        Serial.println("[INFO] Unrecognized command");
      }
    }
  }
}
