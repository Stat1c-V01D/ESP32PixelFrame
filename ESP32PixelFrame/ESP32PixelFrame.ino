/*
 Name:		ESP32PixelFrame.ino
 Created:	23.04.2020 01:27:59
 Author:	Kai
*/

//TODO [QOL] Serial debug messages for all classes/functions
//TODO [QOL] error handler for all classes/functions
#include "Arduino.h"
#include "BMPanim.h"
//#include "font.h"
#ifdef USE_LS
#include "AutoBrightness.h"
#endif // USE_LS
#ifdef USE_RTC_CLOCK
#include "ESPtime.h"
#endif // USE_RTC_CLOCK
#ifdef USE_DHT_SENSOR
#include "ESPdht.h"
#endif // USE_DHT_SENSOR
#ifdef USE_IR_REMOTE
#include "IRcontrol.h"
#endif // USE_IR_REMOTE

//TODO [CODE] store time and states in I2C clock eeprom
Matrix *mtx;
BMPanim *bmp;
AutoBrightness *abc;
ESPtime *esprtc;
data timeClient;
ESPdht *espdht;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial); // wait for serial
	delay(200);
	Serial.println(F("Setup"));
	mtx = new Matrix;
	bmp = new BMPanim(mtx);
	abc = new AutoBrightness(mtx);
	esprtc = new ESPtime(timeClient);
	espdht = new ESPdht;
	Serial.println(F("Setup Finished"));
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.println(F("xxxxxxxxxxxxxxxxLOOPxxxxxxxxxxxxxxxx"));
	Serial.println(F("++++++++++++++++ABC+++++++++++++++++"));
	abc->update();
	Serial.println(F("------------------------------------"));
	Serial.println(F("++++++++++++++++BMP+++++++++++++++++"));
	bmp->animate();
	Serial.println(F("------------------------------------"));
	Serial.println(F("++++++++++++++++TIME++++++++++++++++"));
	esprtc->update();
	Serial.println(F("------------------------------------"));
	Serial.println(F("++++++++++++++++DHT+++++++++++++++++"));
	espdht->update();
	Serial.println(F("------------------------------------"));
	/*
	Serial.println(F("++++++++++++++++FONT++++++++++++++++"));
	not yet working properly
		font _3x5(mtx);
	_3x5.drawNumber(0, 11, "1234");
	delay(10000);
	mtx->clear();
	Serial.println(F("------------------------------------"));
	*/
	Serial.print(F("Free Heap: "));
	Serial.println(ESP.getFreeHeap());
	Serial.println(F("------------------------------------"));
}
