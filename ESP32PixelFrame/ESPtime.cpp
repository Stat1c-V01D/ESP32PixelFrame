#include "ESPtime.h"

ESPtime::ESPtime(data& cfg)
{
	Serial.println("ESPtime constructed");
	_SSID = cfg.cc_SSID;
	_PASSWD = cfg.cc_PASSWD;
	_ntp = cfg.cc_ntp;
	gmtOffset_sec = cfg.gmtOffset_sec;
	daylightOffset_sec = cfg.daylightOffset_sec;
	init();
}

ESPtime::ESPtime(const char * cc_SSID, const char * cc_PASSWD, const char * cc_ntp, const long cl_gmtOffsetsec, const int ci_daylightOffsetsec)
{
	Serial.println("ESPtime constructed");
	_SSID = cc_SSID;
	_PASSWD = cc_PASSWD;
	_ntp = cc_ntp;
	gmtOffset_sec = cl_gmtOffsetsec;
	daylightOffset_sec = ci_daylightOffsetsec;
	init();
}

ESPtime::~ESPtime()
{
}

void ESPtime::init()
{
	//TODO [CODE] Error handling
	Serial.println("ESPtime init");
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}
	if (!rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
	}
	set();
}

void ESPtime::set()
{
	Serial.println("ESPtime set");
	// following line sets the RTC to the date & time this sketch was compiled
	// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	// This line sets the RTC with an explicit date & time, for example to set
	// January 21, 2014 at 3am you would call:
	// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	Serial.printf("Connecting to %s ", _SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(_SSID, _PASSWD);
	int tries = 0;
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		tries++;
		if (tries > 20)
		{
			break;
		}
	}
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println("Failed to obtain time with NTP");
		Serial.println("Setting Time and Date to compiler date");
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		now = rtc.now();
		return;
	}
	else
	{
		Serial.println(" CONNECTED");
		configTime(gmtOffset_sec, daylightOffset_sec, _ntp);
		WiFi.disconnect(true);
		WiFi.mode(WIFI_OFF);
		Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
		rtc.adjust(DateTime(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
		now = rtc.now();
	}
}

void ESPtime::read()
{
	Serial.println("ESPtime read");
	now = rtc.now();
#ifdef DEBUG
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(" (");
	Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
	Serial.print(") ");
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
#endif // DEBUG

}

void ESPtime::update()
{
	Serial.println("ESPtime update");
	read();
	//TODO [CODE] display time
}
