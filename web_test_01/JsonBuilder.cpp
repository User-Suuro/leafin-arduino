#include "JsonBuilder.h"

JsonBuilder::JsonBuilder() : json("{"), firstField(true) {}

JsonBuilder& JsonBuilder::addField(const String& key, const String& value) {
  if (!firstField) json += ",";
  json += "\"" + key + "\":\"" + value + "\"";
  firstField = false;
  return *this;
}

JsonBuilder& JsonBuilder::addField(const String& key, float value, int decimals) {
  if (!firstField) json += ",";
  json += "\"" + key + "\":" + String(value, decimals);
  firstField = false;
  return *this;
}

String JsonBuilder::build() {
  return json + "}";
}
