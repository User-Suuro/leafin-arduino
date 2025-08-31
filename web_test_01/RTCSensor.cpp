#include "RTCSensor.h"

bool RTCSensor::begin(bool forceSetTime) {
    if (!rtc.begin()) return false;

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (forceSetTime || rtc.lostPower()) {
        // Set RTC to the time when the sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    return true;
}

void RTCSensor::setTime(int year, int month, int day, int hour, int minute, int second) {
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
}

String RTCSensor::getTime() {
    DateTime now = rtc.now();
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    return String(buffer);
}

String RTCSensor::getDate() {
    DateTime now = rtc.now();
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", now.year(), now.month(), now.day());
    return String(buffer);
}
