#include <Arduino.h>
#include "WiFiModule.h"
#include "HttpClient.h"
#include "TurbiditySensor.h"
#include "phSensor.h"
#include "RTCSensor.h"
#include "SensorDataBuilder.h"

// WiFi credentials
const char* ssid     = "X8b";
const char* password = "12345678";
const char* host     = "yamanote.proxy.rlwy.net";
const int port       = 16955;
const char* endpoint = "/api/arduino/send-data";

// Sensors
RTCSensor rtcSensor; // SDA | SCL
TurbiditySensor turbiditySensor(A0);
PhSensor phSensor(A1);

// Serial Definitions
#define SerialLog Serial      // USB Serial Monitor
#define SerialESP Serial1     // ESP-01S via Serial1 (TX1=18, RX1=19)

void setup() {
  SerialLog.begin(9600);    
  SerialESP.begin(9600);
  rtcSensor.begin();

  delay(2000);                 
  SerialLog.println("🔧 Starting WiFi connection...");
  connectToWiFi(ssid, password, SerialESP, SerialLog);
}


void loop() {
  // Read sensor data
  float turbidity = turbiditySensor.readVoltage();
  float ph = phSensor.readPH();
  String currentTime = rtcSensor.getTime();
  String currentDate = rtcSensor.getDate();

  // Log sensor values
  SerialLog.print("💧 Turbidity: "); SerialLog.print(turbidity); SerialLog.println(" V");
  SerialLog.print("🧪 pH: "); SerialLog.println(ph);
  SerialLog.print("Time: "); SerialLog.println(currentTime);
  SerialLog.print("Date: "); SerialLog.println(currentDate);

  // Build JSON payload here
  SensorDataBuilder builder;
  builder.addField("connected", "true")
         .addField("time", currentTime)
         .addField("date", currentDate)
         .addField("ph", ph)
         .addField("turbid", turbidity);

  String jsonPayload = builder.build();

  // Send JSON to server
  sendAllSensorDataToServer(host, port, endpoint, SerialESP, jsonPayload, SerialLog);

  delay(5000);
}
