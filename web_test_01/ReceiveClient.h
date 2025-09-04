#ifndef RECEIVECLIENT_H
#define RECEIVECLIENT_H

#include <Arduino.h>

String receiveCommandFromWeb(
  const char* host,
  int port,
  const char* endpoint,
  Stream& espSerial,
  Stream& logSerial
);

#endif
