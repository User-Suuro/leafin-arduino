#pragma once

void makeHTTPRequest(const char* host, int port, const char* endpoint, Stream& espSerial);
void sendWeightToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float weight);

