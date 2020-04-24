#pragma once
//TODO [DEBUG] test behavior/function
#include "config.h"
#include <RTClib.h>
#include <WiFi.h>
#include <time.h>
class ESPtime
{
public:
	ESPtime(struct data&);
	ESPtime(const char* cc_SSID = "NO_SSID", const char* cc_PASSWD = "NO_PASS", const char* cc_ntp = "pool.ntp.org", long cl_gmtOffsetsec = 3600, int ci_daylightOffsetsec = 3600);
	~ESPtime();
	void init();
	void set(); //TODO [FIX] call NTP sync periodically or if new time is "less" than old time;
	void read();
	void update();
protected:
	DateTime now;
	struct tm timeinfo;
private:
	char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
#ifdef DS1307
#define DS1307_ADDRESS RTC_I2C_ADDRESS
	RTC_DS1307 rtc;
#endif // DS1307
#ifdef DS3231
#define DS3231_ADDRESS RTC_I2C_ADDRESS
	RTC_DS3231 rtc;
#endif // DS3231
#ifdef PCF8523
#define PCF8523_ADDRESS RTC_I2C_ADDRESS
	RTC_PCF8523 rtc;
#endif // PCF8523
	const char* _SSID;
	const char* _PASSWD;
	const char* _ntp;
	long  gmtOffset_sec;
	int   daylightOffset_sec;
};