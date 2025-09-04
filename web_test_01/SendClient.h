#ifndef SENDCLIENT_H
#define SENDCLIENT_H

#include <Arduino.h>

void sendDataToServer(
  const char* host,
  int port,
  const char* endpoint,
  Stream& espSerial,
  const String& jsonPayload,
  Stream& logSerial
);


#endif
