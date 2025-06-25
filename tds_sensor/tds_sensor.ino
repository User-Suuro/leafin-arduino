#include <EEPROM.h>
#include "GravityTDS.h"
#define TDSSensorPin A1
GravityTDS gravityTds;

float temperature = 25,tdsValue = 0;

void setup() {
  Serial.begin(115200);
  gravityTds.setPin(TDSSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  delay(1000);
}
