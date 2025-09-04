#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>   // For DS3231/DS1307

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change to 0x3F if needed
RTC_DS3231 rtc;

void setup() {
  Wire.begin();       // Start I2C on pins 20/21
  lcd.init();         
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("RTC + LCD OK");
}

void loop() {
  DateTime now = rtc.now();
  
  lcd.setCursor(0, 1);
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());

  delay(1000);
}
