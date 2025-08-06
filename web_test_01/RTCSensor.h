#ifndef RTC_SENSOR_H
#define RTC_SENSOR_H

#include <RTClib.h>

class RTCSensor {
  private:
    RTC_DS3231 rtc;

  public:
    bool begin();
    String getTime();
    String getDate();
};

#endif
