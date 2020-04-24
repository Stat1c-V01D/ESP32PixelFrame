#include "Matrix.h"

Matrix::Matrix()
{
	Serial.println("Matrix constructed");
	FastLED.addLeds<CHIPSET, DATApin, RGBorder>(leds, MATRIX_WIDTH * MATRIX_HEIGHT).setCorrection(Correction);
	init();
}

Matrix::Matrix(int i_width, int i_height, int i_rotation) 
{
	Serial.println("Matrix constructed");
	FastLED.addLeds<CHIPSET, DATApin, RGBorder>(leds, i_width * i_height).setCorrection(Correction);
	init();
}

Matrix::~Matrix()
{
	//delete fled;
}

void Matrix::init()
{
	Serial.println("Matrix init");
#ifdef USE_BUTTONS
	pinMode(BUTTON1_PIN, INPUT_PULLUP);
	pinMode(BUTTON2_PIN, INPUT_PULLUP);
	pinMode(BUTTON3_PIN, INPUT_PULLUP);
#endif // USE_BUTTONS
	_rotation = MATRIX_ROTATION;
	brightness(BRIGHTNESS);
#ifdef MATRIX_POWERLIMIT
	setMaxpower();
#endif // MATRIX_POWERLIMIT
	clear();
}

void Matrix::brightness(int i_val)
{
	Serial.println("Matrix setting brightness");
	FastLED.setBrightness(i_val);
}

void Matrix::burnTest()
{
	FastLED.setBrightness(255);
	//setMaxpower(5, 20000);
	for (int i = 0; i < 256; i++)
	{
		leds[i] = CRGB::White;
	}
	FastLED.show();
}

void Matrix::setMaxpower(uint8_t volts, uint32_t milliamps)
{
	if (volts != 0 && milliamps != 0)
	{
		FastLED.setMaxPowerInVoltsAndMilliamps(volts, milliamps);
		return;
	}
#ifdef MATRIX_POWERLIMIT
	FastLED.setMaxPowerInVoltsAndMilliamps(MAX_VOLT, MAX_MILLIAMPS);
#endif // MATRIX_POWERLIMIT
}

void Matrix::clear()
{
	Serial.println("Matrix clearing");
	for (int i = 0; i < numLEDs; i++)
	{
		leds[i] = CRGB::Black;
	}
	FastLED.show();
}

void Matrix::show()
{
	FastLED.show();
}

int Matrix::xyToIndex(int i_x, int i_y)
{
	int pos;
	if (i_y % 2) {
		pos = (i_y - 1) * MATRIX_WIDTH + i_x - 1;
	}
	else {
		pos = (i_y - 1) * MATRIX_WIDTH + MATRIX_WIDTH - i_x;
	}
	return pos;
}

void Matrix::setPixel(byte by_x, byte by_y, CRGB color) {
	int pos;
	if (by_y % 2) {
		pos = (by_y - 1) * MATRIX_WIDTH + by_x - 1;
	}
	else {
		pos = (by_y - 1) * MATRIX_WIDTH + MATRIX_WIDTH - by_x;
	}
	leds[pos] = color;
}

CRGB Matrix::getPixel(byte by_x, byte by_y) {
	int pos;
	if (by_y % 2) {
		pos = (by_y - 1) * MATRIX_WIDTH + by_x - 1;
	}
	else {
		pos = (by_y - 1) * MATRIX_WIDTH + MATRIX_WIDTH - by_x;
	}
	return leds[pos];
}

void Matrix::swapPixel(byte by_x1, byte by_y1, byte by_x2, byte by_y2) {
	CRGB tempColor = getPixel(by_x1, by_y1);
	setPixel(by_x1, by_y1, getPixel(by_x2, by_y2));
	setPixel(by_x2, by_y2, tempColor);
}

void Matrix::update() {
	for (int n = 0; n < _rotation / 90; n++) {
		for (int i = 1; i <= MATRIX_WIDTH; i++) {
			for (int j = i + 1; j <= MATRIX_HEIGHT; j++) {
				swapPixel(i, j, j, i);
			}
		}
		for (int i = 1; i <= MATRIX_WIDTH; i++) {
			for (int j = 1; j <= MATRIX_HEIGHT / 2; j++) {
				swapPixel(i, j, i, MATRIX_HEIGHT - j + 1);
			}
		}
	}
	FastLED.show();
}

void Matrix::readButtons()
{
	//TODO [CODE] assign interrupt or read periodically and handle input
#ifdef USE_BUTTONS
	int reading;
	reading = digitalRead(BUTTON1_PIN);
	if (reading != _lastButtonState1) {
		// reset the debouncing timer
		_lastDebounceTime1 = millis();
	}

	if ((millis() - _lastDebounceTime1) > _debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:
		if (reading != _button1) {
			_button1 = reading;
		}
	}
	reading = digitalRead(BUTTON2_PIN);
	if (reading != _lastButtonState2) {
		// reset the debouncing timer
		_lastDebounceTime2 = millis();
	}

	if ((millis() - _lastDebounceTime2) > _debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:
		if (reading != _button2) {
			_button2 = reading;
		}
	}
	reading = digitalRead(BUTTON3_PIN);
	if (reading != _lastButtonState3) {
		// reset the debouncing timer
		_lastDebounceTime3 = millis();
	}

	if ((millis() - _lastDebounceTime3) > _debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:
		if (reading != _button3) {
			_button3 = reading;
		}
	}
#endif // USE_BUTTONS
#ifndef USE_BUTTONS
	_button1, _button2, _button3 = HIGH;
#endif // !USE_BUTTONS
}

void Matrix::loadPgmImage(const long *image) {
	for (byte column = 1, counter = 0; column <= MATRIX_HEIGHT; column++) //Set the right pixels in the led matrix based on the array chosen above.
	{
		for (byte row = 1; row <= MATRIX_WIDTH; row++, counter++)
		{
			setPixel(row, column, image[counter]);
		}
	}
}

void Matrix::showError(const long *image) {
	loadPgmImage(image);
	FastLED.show();
}
