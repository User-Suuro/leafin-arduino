#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <Arduino.h>

bool isWiFiConnected(Stream& espSerial);
void connectToWiFi(const char* ssid, const char* password, Stream& espSerial, Stream& logSerial);

#endif
