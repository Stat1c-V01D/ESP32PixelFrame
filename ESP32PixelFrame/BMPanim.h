#pragma once
#include "config.h"
#include "Matrix.h"
#include "FileFunctions.h"
class BMPanim 
{
public:
	BMPanim(Matrix *pmatrix);
	~BMPanim();
	void init();
	void animate();
	void processFolder(const char* cc_path);
	void processBMP(const char* cc_path);
	//void processBMP(String s_path);
	void getInfo(String s_path);
	bool breakout = false;
private:
	long _startanimTime,_processTime;
	long _animationDuration = ANIMATION_DURATION;
	CRGB _timeColor, _tempColor, _humidColor;
	FileFunctions *_fileHelper = new FileFunctions;
	File _bmpFile;
	Matrix *_matrix;
	IniFile *p_ini;
	const char *_cfgfile = "/config.ini";
	struct animInfo
	{
		int _frames,_hold,_moveX,_moveY;
		bool _loopAnim,_loopMove,_panoff;
		String _nextfolder = "";
	}animation;
};

//TODO [ENHANCEMENT] not yet implemented: moveX -- moveY -- loopMove -- panoff -- nextfolder