/*
 Name:		ESP32PixelFrame.ino
 Created:	23.04.2020 01:27:59
 Author:	Kai
*/

//TODO [QOL] Serial debug messages for all classes/functions

#include "Arduino.h"
#include "BMPanim.h"
#ifdef USE_LS
#include "AutoBrightness.h"
#endif // USE_LS
#ifdef USE_DHT_SENSOR
#include "ESPdht.h"
#endif // USE_DHT_SENSOR
#ifdef USE_RTC_CLOCK
#include "ESPtime.h"
#endif // USE_RTC_CLOCK
#ifdef USE_IR_REMOTE
#include "IRcontrol.h"
#endif // USE_IR_REMOTE

struct data
{
	const char* cc_SSID = "YOUR_SSID";
	const char* cc_PASSWD = "YOUR_PASS";
	const char* cc_ntp = "pool.ntp.org";
	long  gmtOffset_sec = 3600;
	int   daylightOffset_sec = 3600;
}timeClient;


BMPanim *bmp;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial); // wait for serial
	delay(200);
	Serial.println("Setup");
	bmp = new BMPanim;
	Serial.println("Setup Finished");
}
//String animfolder = "/RGBCrash/";
//const char * af = "/RGBCrash";
// the loop function runs over and over again until power down or reset
void loop() {
	Serial.println("------------LOOP------------");
	//delay(1000);
	//Serial.println("Loading Folder");
	//bmp->processFolder(af);
	bmp->animate();
	Serial.print("Free Heap: ");
	Serial.println(ESP.getFreeHeap());
	//delay(5000);
}

/*
void singlebmp()
{
	Serial.println("Loading image");
	String path;
	for (int i = 0; i < 22; i++)
	{
		path = animfolder + String(i);
		bmp->processBMP(path);
		delay(50);
	}
}
*/