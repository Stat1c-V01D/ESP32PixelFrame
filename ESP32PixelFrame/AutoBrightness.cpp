#include "AutoBrightness.h"

AutoBrightness::AutoBrightness()
{
	_lsPIN = LS_PIN;
	init();
}

AutoBrightness::AutoBrightness(int i_sensorPin, float f_bmin, float f_bmax, float f_minsense, float f_maxsense)
{
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
	pinMode(_lsPIN, INPUT);
}

void AutoBrightness::read()
{
	_lsV = analogRead(_lsPIN);
	_lsVarr[_lsVcount] = _lsV;
	_lsVcount++;
	if (_lsVcount >= 100)
	{
		_lsVcount = 0;
	}
	_lsV = 0;
	for (int i = 0; i < 100; i++)
	{
		_lsV += _lsVarr[i];
	}
	_lsV /= 100;
}

void AutoBrightness::update(uint8_t ui_scale)
{
	read();
	_targetB = ((_lsV - _maxSense) / (_minSense - _maxSense)) * (_bMin - _bMax) + _bMax;
	if (_lsV > _minSense) {
		_targetB = 5;
	}
	if (_lsV < _maxSense) {
		_targetB = _bMax;
	}
	_targetB = ui_scale;
	_matrix.brightness(_targetB);
}
