#pragma once
#include "config.h"
#include <FastLED.h>
#include "images.h"
//#include "fonts.h"

class Matrix :public CFastLED
{
public:
	Matrix();
	Matrix(int i_width, int i_height, int i_rotation = 0);
	~Matrix();
	void init();
	void brightness(int i_val = 255);
	void burnTest(); //be careful with that
	void setMaxpower(uint8_t volts = 0,uint32_t  milliamps = 0); //overrides brightness and Fastled delay
	void clear();
	void show();
	int xyToIndex(int i_x, int i_y);
	void setPixel(byte by_x, byte by_y, CRGB color);
	CRGB getPixel(byte by_x, byte by_y);
	void swapPixel(byte by_x1, byte by_y1, byte by_x2, byte by_y2);
	void showError(const long *image);
	void update();
	void readButtons();
	//void draw(fonts * p_font, String str_id ,float f_value); Protoype for now
	//CFastLED *fled = new CFastLED;
	//CFastLED FastLED;
	CRGB leds[numLEDs];
protected:
	int _button1, _button2, _button3;
	

private:
	int _rotation;
	void loadPgmImage(const long *image);
	unsigned long _lastDebounceTime1, _lastDebounceTime2, _lastDebounceTime3 = 0;
	int _lastButtonState1, _lastButtonState2, _lastButtonState3 = HIGH;
	unsigned long _debounceDelay = 50;
};