#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Network
#include "WiFiModule.h"
#include "SendClient.h"
#include "ReceiveClient.h"
#include "JsonBuilder.h"

// Sensors
#include "FloatSwitch.h"
#include "TurbiditySensor.h"
#include "phSensor.h"
#include "RTCSensor.h"
#include "DS18B20Sensor.h"
#include "MQ137Sensor.h"
#include "TDS_Sensor.h"

// Config
const char* ssid="X8b";
const char* password="12345678";
const char* host = "yamanote.proxy.rlwy.net";
const int port   = 16955;

// Endpoints
const char* send_enpoint    = "/api/arduino/send-data"; 
const char* receive_endpoint = "/api/arduino/send-command";
const char* reply_endpoint = "/api/arduino/command-reply";

// Task Scheduler
unsigned long previousMillisSendData = 0;
unsigned long previousMillisReceiveData = 0;
unsigned long previousMillisRenderLCD = 0;

const long intervalSendData = 10000;
const long intervalReceiveData = 5000;
const long intervalRenderLCD = 1000;

// States

float turbidity_val;
float ph_val;
String time_val;
String date_val;
float temp_val;
float tdsValue;
bool isWaterLevelTriggered;
float nh3_gas;

// SDA | SCL
RTCSensor rtcSensor; // probably at 0x3a
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Analog Pins
MQ137Sensor nh3Gas(A0);
TurbiditySensor turbidity(A1);
pHSensor ph(A2);
TDSSensor tds(A3);          

// Digital Pins

// 2 -> DT -> HX711
// 3 -> SCK -> HX711

const int RELAY_MINI_PUMP = 4;
const int RELAY_MAIN_PUMP = 5;

FloatSwitch waterLevel(6, true);
DS18B20Sensor waterTemp(7);

// 8 -> Servo Feeder 
// 9 -> Servo Tank
// 10 -> D0 -> MQ137

// 22 -> D1 -> NPK
// 23 -> DE -> NPK
// 24 -> RE -> NPK
// 25 -> RO -> NPK

// Serial Definitions
#define SerialLog Serial
#define SerialESP Serial1

void setup() {
    Wire.begin();
    lcd.init();
    lcd.backlight();

    SerialLog.begin(9600);    
    SerialESP.begin(9600);

    rtcSensor.begin(true);
    waterTemp.begin();
    waterLevel.begin();
    ph.begin();
    tds.begin();
    
    pinMode(RELAY_MINI_PUMP, OUTPUT);
    pinMode(RELAY_MAIN_PUMP, OUTPUT);
  
    nh3Gas.setCalibration(10.0, -0.263, 0.42);  

    // Try connecting until success
    while (!isWiFiConnected(SerialESP)) {
        SerialLog.println("🔧 Starting WiFi connection...");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Connecting to");
        lcd.setCursor(0, 1);
        lcd.print("WiFi...");

        connectToWiFi(ssid, password, SerialESP, SerialLog);
        delay(2000);
    }

    // Once connected
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected!");
}

void loop() {

   unsigned long currentMillis = millis();

    if (currentMillis - previousMillisReceiveData >= intervalReceiveData) {
        previousMillisReceiveData = currentMillis;
        receiveVal();
    }

    if (currentMillis - previousMillisSendData >= intervalSendData) {
        previousMillisSendData = currentMillis;
        sendVal();
    }

    if (currentMillis - previousMillisRenderLCD >= intervalRenderLCD) {
        previousMillisRenderLCD = currentMillis;

        readSensors();

        // do lcd rendering
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("1: pH: ");
        lcd.setCursor(0,1);
        lcd.print(ph_val);
    }

}

// -- receive values from web -- //

void receiveVal() {
    String webCommand = receiveCommandFromWeb(host, port, receive_endpoint, SerialESP, SerialLog);

    if (webCommand.length() > 0) {
        // Parse JSON here
        StaticJsonDocument<256> doc;
        DeserializationError err = deserializeJson(doc, webCommand);
        if (err) {
            SerialLog.print("❌ JSON parse failed: ");
            SerialLog.println(err.c_str());
            return;
        }

        // Read fields from JSON
        const char* relay_mini_pump = doc["relay_mini_pump"];
        const char* relay_main_pump = doc["relay_main_pump"];

        SerialLog.print("Mini Pump: ");
        SerialLog.println(relay_mini_pump ? relay_mini_pump : "null");

        SerialLog.print("Main Pump: ");
        SerialLog.println(relay_main_pump ? relay_main_pump : "null");

        // Control relay pins
        if (relay_mini_pump && String(relay_mini_pump) == "on") {
            digitalWrite(RELAY_MINI_PUMP, HIGH);
        } else {
            digitalWrite(RELAY_MINI_PUMP, LOW);
        }

        if (relay_main_pump && String(relay_main_pump) == "on") {
            digitalWrite(RELAY_MAIN_PUMP, HIGH);   // ✅ fixed
        } else {
            digitalWrite(RELAY_MAIN_PUMP, LOW);    // ✅ fixed
        }

        // --- SEND ACK TO SERVER ---
        StaticJsonDocument<128> replyDoc;
        replyDoc["status"] = "success";
        replyDoc["relay_mini_pump"] = relay_mini_pump ? relay_mini_pump : "null";
        replyDoc["relay_main_pump"] = relay_main_pump ? relay_main_pump : "null";

        String replyJson;
        serializeJson(replyDoc, replyJson);

        sendDataToServer(host, port, reply_endpoint, SerialESP, replyJson, SerialLog);

        SerialLog.println("✅ Command executed and reply sent");
    } else {
        SerialLog.println("⚠ No command received");
    }
}



void readSensors() {
    turbidity_val = turbidity.getValue();
    ph_val = ph.readPH();
    time_val = rtcSensor.getTime();
    date_val = rtcSensor.getDate();
    temp_val = waterTemp.readCelsius();
    tds.update(temp_val);
    tdsValue = tds.getValue();
    isWaterLevelTriggered = waterLevel.isTriggered();
    nh3_gas = nh3Gas.readPPM();
}

// -- send values to web -- //

void sendVal() {
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

    Serial.print("NH3 gas PPM: "); Serial.println(nh3_gas);

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
    sendDataToServer(host, port, send_enpoint, SerialESP, jsonPayload, SerialLog);

}

