#pragma once
//TODO [DEBUG] test behavior/function
#include "config.h"
#include "Matrix.h"
class AutoBrightness :public Matrix
{
public:
	AutoBrightness();
	AutoBrightness(int i_sensorPin, float f_bmin, float f_bmax, float f_minsense, float f_maxsense);
	~AutoBrightness();
	void init();
	void read();
	void update(uint8_t ui_scale = 0);
private:
	int _lsVarr[100] = { 0 };
	byte _lsVcount = 0;
	int _lsV, _lsPIN;
	float _targetB, _bMin = 5, _bMax = 70, _minSense = 3000, _maxSense = 1000;
	Matrix _matrix;
};



