#pragma once
#include <Arduino.h>  // ✅ Add this so Stream and String work

void makeHTTPRequest(const char* host, int port, const char* endpoint, Stream& espSerial);
void sendWeightToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float weight);
