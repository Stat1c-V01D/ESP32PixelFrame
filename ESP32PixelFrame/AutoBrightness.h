#pragma once
//TODO [DEBUG] test behavior/function
#include "config.h"
#include "Matrix.h"
class AutoBrightness
{
public:
	AutoBrightness(Matrix * pmatrix);
	AutoBrightness(Matrix * pmatrix,int i_sensorPin, float f_bmin, float f_bmax, float f_minsense, float f_maxsense);
	~AutoBrightness();
	void init();
	void read();
	void update(uint8_t ui_scale = 0);
private:
	int  _minSense = 300, _maxSense = 3000, _lsPIN, _lsVarr[100] = { 0 };
	uint8_t _bMin = 15, _bMax = 255, _targetB;
	long _lsV;
	Matrix *_matrix;
};



