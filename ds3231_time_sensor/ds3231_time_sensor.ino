#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("❌ Couldn't find DS3231 RTC module");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("⚠️ RTC lost power, setting the time!");
    // Set the time once, then comment out or remove this line
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Use PC compile time
    // rtc.adjust(DateTime(2025, 8, 5, 15, 30, 0)); // Manual set: YYYY, MM, DD, HH, MM, SS
  }
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("📅 Date: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);

  Serial.print(" 🕒 Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  delay(1000);
}
