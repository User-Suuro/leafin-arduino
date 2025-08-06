#ifndef SENSOR_DATA_BUILDER_H
#define SENSOR_DATA_BUILDER_H

#include <Arduino.h>

class SensorDataBuilder {
  String json;
  bool firstField;
public:
  SensorDataBuilder() : json("{"), firstField(true) {}

  SensorDataBuilder& addField(const String& key, const String& value) {
    if (!firstField) json += ",";
    json += "\"" + key + "\":\"" + value + "\"";
    firstField = false;
    return *this;
  }

  SensorDataBuilder& addField(const String& key, float value, int decimals = 2) {
    if (!firstField) json += ",";
    json += "\"" + key + "\":" + String(value, decimals);
    firstField = false;
    return *this;
  }

  String build() {
    return json + "}";
  }
};

#endif
