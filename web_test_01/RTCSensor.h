#ifndef RTC_SENSOR_H
#define RTC_SENSOR_H

#include <RTClib.h>

class RTCSensor {
  private:
    RTC_DS3231 rtc;

  public:
    // Allow forcing time set on startup
    bool begin(bool forceSet = false);

    // Manual time setting function
    void setTime(int year, int month, int day, int hour, int minute, int second);

    // Getters
    String getTime();
    String getDate();
};

#endif
