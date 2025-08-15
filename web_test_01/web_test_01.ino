#include <Arduino.h>
#include "WiFiModule.h"
#include "HttpClient.h"
#include "TurbiditySensor.h"
#include "phSensor.h"
#include "RTCSensor.h"
#include "SensorDataBuilder.h"
#include "DS18B20Sensor.h"
#include "FloatSwitch.h"

// WiFi credentials
const char* ssid     = "HpElitebook";
const char* password = "12345678";
const char* host     = "yamanote.proxy.rlwy.net";
const int port       = 16955;
const char* send_enpoint = "/api/arduino/send-data";
const char* receive_endpoint = "/api/arduino/receive-data";

// SDA | SCL
RTCSensor rtcSensor;

// Analog Pins
TurbiditySensor turbiditySensor(A0);
pHSensor phSensor(A1);

// Digital Pins
#define FLOAT_SWITCH_PIN 8
FloatSwitch waterLevel(FLOAT_SWITCH_PIN, true);

#define DS18B20_PIN 9
DS18B20Sensor waterTemp(DS18B20_PIN);

// Serial Definitions
#define SerialLog Serial      // USB Serial Monitor
#define SerialESP Serial1     // ESP-01S via Serial1 (TX1=18, RX1=19)

void setup() {
  SerialLog.begin(9600);    
  SerialESP.begin(9600);

  
  rtcSensor.begin(true);
  waterTemp.begin();
  waterLevel.begin();
  phSensor.begin();

  delay(1000);  
                 
  SerialLog.println("🔧 Starting WiFi connection...");
  connectToWiFi(ssid, password, SerialESP, SerialLog);

  delay(1000);

  if (waterTemp.isConnected()) {
      Serial.println("DS18B20 sensor connected successfully!");
  } else {
      Serial.println("No DS18B20 sensor detected!");
  }

  delay(1000)

  if (!phSensor.isConnected()) {
      Serial.println("⚠ pH sensor not detected!");
  } else {
      Serial.println("✅ pH sensor connected.");
  }
}

void loop() {
  float turbidity = turbiditySensor.readVoltage();
  float phValue = phSensor.readPH(); // Call readPH() from the object
  String currentTime = rtcSensor.getTime();
  String currentDate = rtcSensor.getDate();
  float tempC = waterTemp.readCelsius();
  bool isWaterLevelTriggered = waterLevel.isTriggered();

  SerialLog.print("💧 Turbidity: "); SerialLog.print(turbidity); SerialLog.println(" V");
  SerialLog.print("🧪 pH: "); SerialLog.println(phValue);
  SerialLog.print("Time: "); SerialLog.println(currentTime);
  SerialLog.print("Date: "); SerialLog.println(currentDate);
  SerialLog.print("Water Temperature: "); SerialLog.println(tempC);

   if (waterLevel.isTriggered()) {
        Serial.println("⚠ Insufficient Water Level");
    } else {
        Serial.println("Water level normal.");
    }

  SensorDataBuilder builder;
  builder.addField("connected", true)
         .addField("time", currentTime)
         .addField("date", currentDate)
         .addField("ph", phValue)
         .addField("turbid", turbidity)
         .addField("water_temp", tempC)
         .addField("is_water_lvl_normal", !isWaterLevelTriggered);

  String jsonPayload = builder.build();
  sendAllSensorDataToServer(host, port, send_enpoint, SerialESP, jsonPayload, SerialLog);

  delay(5000);
}

