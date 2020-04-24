#include "AutoBrightness.h"

AutoBrightness::AutoBrightness(Matrix *pmatrix)
{
	Serial.println("AutoBrightness constructed");
	_matrix = pmatrix;
	_lsPIN = LS_PIN;
	init();
}

AutoBrightness::AutoBrightness(Matrix *pmatrix,int i_sensorPin, float f_bmin, float f_bmax, float f_minsense, float f_maxsense)
{
	_matrix = pmatrix;
	_lsPIN = i_sensorPin;
	_bMin = f_bmin;
	_bMax = f_bmax;
	_minSense = f_minsense;
	_maxSense = f_maxsense;
	init();
}

AutoBrightness::~AutoBrightness()
{
}

void AutoBrightness::init()
{
	Serial.println("AutoBrightness init");
	pinMode(_lsPIN, INPUT);
}

void AutoBrightness::read()
{
	Serial.println("AutoBrightness read");
	for (int i = 0; i < 100; i++)
	{
		_lsV = analogRead(_lsPIN);
		_lsVarr[i] = _lsV;
	}
}

void AutoBrightness::update(uint8_t ui_scale)
{
	Serial.println("AutoBrightness update");
	if (ui_scale != 0)
	{
		_targetB = ui_scale;
	}
	else
	{
		read();
		for (int i = 0; i < 100; i++)
		{
			_lsV += _lsVarr[i];
		}
		_lsV /= 100;
		//Serial.print("ls value: ");
		//Serial.println(_lsV);
		_targetB = map(_lsV, _minSense, _maxSense, _bMin, _bMax);
		//Serial.print("target: ");
		//Serial.println(_targetB);
		if (_targetB < _bMin) {
			_targetB = _bMin;
		}
		if (_targetB > _bMax) {
			_targetB = _bMax;
		}
	}
	Serial.print("New brightness: ");
	Serial.println(_targetB);
	_matrix->brightness(_targetB);
}
