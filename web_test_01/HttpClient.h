#pragma once
#include <Arduino.h>

void sendConnectionStatus(const char* host, int port, const char* endpoint, Stream& espSerial);
void sendWeightToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float weight);
void sendTurbidityToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float voltage);
void sendPhToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float phValue);

