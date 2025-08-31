#ifndef ReceiveClient_h
#define ReceiveClient_h

#include <Arduino.h>

String receiveCommandFromWeb(
  const char* host,
  int port,
  const char* endpoint,
  Stream& espSerial,
  Stream& logSerial
);

#endif
