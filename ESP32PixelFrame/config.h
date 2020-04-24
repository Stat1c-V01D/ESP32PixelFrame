#pragma once
//setup the dimensions of the LED matrix
#define MATRIX_WIDTH   16
#define MATRIX_HEIGHT  16 
#define MATRIX_ROTATION 90 // 0 -- 90 -- 180  -- 270
//#define MATRIX_POWERLIMIT

//setup FastLED
#define BRIGHTNESS 255 //0-255
#ifdef MATRIX_POWERLIMIT
#define MAX_VOLT 5
#define MAX_MILLIAMPS 10000
#endif // MATRIX_POWERLIMIT
#define CHIPSET WS2812
#define RGBorder GRB
#define DATApin 13
#define Correction TypicalSMD5050
#define numLEDs 256

//Button 1 changes animation/clock mode, Button 2 skips the current animation,
//Button 3 pauses the animation progression, so that the current aniamtion will be played in a loop.
//#define USE_BUTTONS 
#define BUTTON1_PIN 25
#define BUTTON2_PIN 26
#define BUTTON3_PIN 27

//how long each animation should be played in animation mode (ms)
#define ANIMATION_DURATION 10000

//automatically cycle between clockmode and animationmode (seconds)
//set to 0 to disable
#define AUTO_CLOCK_DURATION 0

// the CS for connecting the SD card
#define SD_PIN  4

//activates automatic brightness control 
#define USE_LS
#define LS_PIN 34

//activates clock mode
#define USE_RTC_CLOCK 
#define DS1307 //clock type
#define RTC_I2C_ADDRESS 0x68 // I2C adress of  RTC
#define EEPROM_I2C_ADDRESS 0x50 // I2C address of RTCs EEPROM
struct data
{
	const char* cc_SSID = "YOUR_SSID";
	const char* cc_PASSWD = "YOUR_PASS";
	const char* cc_ntp = "pool.ntp.org";
	long  gmtOffset_sec = 3600;
	int   daylightOffset_sec = 3600;
};

//display ambbient temperature and humidity in clock mode
#define USE_DHT_SENSOR
#define DHT_TYPE DHT22 //dht type
#define DHT_PIN 14 //data Pin

//#define USE_IR_REMOTE
#define IR_PIN 32

//print additional information to the serial monitor
//TODO [QOL] better debug levels
#define DEBUG 0