#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include <Arduino.h>

class JsonBuilder {
  String json;
  bool firstField;

public:
  JsonBuilder();

  JsonBuilder& addField(const String& key, const String& value);
  JsonBuilder& addField(const String& key, float value, int decimals = 2);

  String build();
};

#endif
