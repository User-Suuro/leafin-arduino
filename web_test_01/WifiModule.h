#pragma once

#include <Arduino.h>

void connectToWiFi(const char* ssid, const char* password, Stream& espSerial);
void sendCommand(const String& cmd, Stream& espSerial, int delayMs = 2000, bool showResponse = true);
