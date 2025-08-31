#include <Arduino.h>

// network
#include "WiFiModule.h"
#include "SendClient.h"
#include "ReceiveClient.h"
#include "JsonBuilder.h"

// sensors
#include "FloatSwitch.h"
#include "TurbiditySensor.h"
#include "phSensor.h"
#include "RTCSensor.h"
#include "DS18B20Sensor.h"
#include "MQ137Sensor.h"
#include "TDS_Sensor.h"

const char* ssid="X8b";
const char* password="12345678";

const char* host = "yamanote.proxy.rlwy.net";
const int port   = 16955;

// Endpoints
const char* send_enpoint    = "/api/arduino/send-data";
const char* receive_endpoint = "/api/arduino/send-command";

// SDA | SCL
RTCSensor rtcSensor;

// Analog Pins
MQ137Sensor nh3Gas(A0);
TurbiditySensor turbidity(A1);
pHSensor ph(A2);
TDSSensor tds(A3);

// Digital Pins
FloatSwitch waterLevel(6, true);
DS18B20Sensor waterTemp(7);

// Serial Definitions
#define SerialLog Serial
#define SerialESP Serial1

void setup() {

    SerialLog.begin(9600);    
    SerialESP.begin(9600);
    rtcSensor.begin(true);
    waterTemp.begin();
    waterLevel.begin();
    ph.begin();
    tds.begin();
    
    nh3Gas.setCalibration(10.0, -0.263, 0.42);  

    connectToWiFi(ssid, password, SerialESP, SerialLog);

    delay(2000);

    while (!isWiFiConnected(SerialESP)) {
        SerialLog.println("🔧 Starting WiFi connection...");
        connectToWiFi(ssid, password, SerialESP, SerialLog);
        delay(2000);
    }
}

void loop() {

   sendVal();

   receiveVal();
 
   delay(5000); // keep as 5s cycle
}

// -- receive values from web -- //

void receiveVal() {
    String webCommand = receiveCommandFromWeb(host, port, receive_endpoint, SerialESP, SerialLog);

    if (webCommand.length() > 0) {
        SerialLog.println("✅ Command from web: " + webCommand);

        // Example: If command is "hello", you could trigger an LED or action
        if (webCommand == "hello") {
            SerialLog.println("🌟 Hello command received!");
        }
    } else {
        SerialLog.println("⚠ No command received");
    }
}

// -- send values to web -- //

void sendVal() {

    float turbidity_val = turbidity.readNTU();
    float ph_val = ph.readPH();

    String time_val = rtcSensor.getTime();
    String date_val = rtcSensor.getDate();

    float temp_val = waterTemp.readCelsius();

    tds.update(temp_val);
    float tdsValue = tds.getValue();

    bool isWaterLevelTriggered = waterLevel.isTriggered();
    float nh3_gas = nh3Gas.readPPM();

    SerialLog.print("💧 Turbidity: "); SerialLog.println(turbidity_val);
    SerialLog.print("🧪 pH: "); SerialLog.println(ph_val);
    SerialLog.print("Time: "); SerialLog.println(time_val);
    SerialLog.print("Date: "); SerialLog.println(date_val);
    SerialLog.print("Water Temperature: "); SerialLog.println(temp_val);
    SerialLog.print("TDS: "); SerialLog.println(tdsValue, 0);
    
    if (waterLevel.isTriggered()) {
        Serial.println("⚠ Insufficient Water Level");
    } else {
        Serial.println("Water level normal.");
    }

    Serial.print("NH3 gas PPM: "); Serial.print(nh3_gas);

    // --- SEND DATA TO SERVER ---
    JsonBuilder builder;
    builder.addField("connected", true)
           .addField("time", time_val)
           .addField("date", date_val)
           .addField("ph", ph_val)
           .addField("turbid", turbidity_val)
           .addField("water_temp", temp_val)
           .addField("tds", tdsValue)
           .addField("float_switch", !isWaterLevelTriggered)
           .addField("nh3_gas", nh3_gas)
           ;

    String jsonPayload = builder.build();
    sendAllSensorDataToServer(host, port, send_enpoint, SerialESP, jsonPayload, SerialLog);

}

