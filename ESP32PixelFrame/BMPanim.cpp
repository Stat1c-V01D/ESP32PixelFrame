#include "BMPanim.h"

BMPanim::BMPanim()
{
	Serial.println("BMPanim constructed");
	init();
}

BMPanim::~BMPanim()
{
	delete _matrix;
	delete _fileHelper;
}

void BMPanim::init()
{
	Serial.println("BMPanim init");
	if (_fileHelper->foldercount == 0)
	{
		_fileHelper->foldercount = _fileHelper->getFoldercount();
	}
	//_matrix = new Matrix;
	//_bmphelper = new FileFunctions;
	//_bmpFile = new File;
}

void BMPanim::animate()
{
	processFolder(_fileHelper->getFolderpath(random(1, _fileHelper->foldercount)).c_str());
}

void BMPanim::processBMP(const char* cc_path)
{
	_processTime = millis();
	//Serial.println("BMPanim processBMP");
	_bmpFile = *_fileHelper->readBMP(cc_path);
	if (!_bmpFile)
	{
		for (int i = 0; i < numLEDs; i++)
		{
			_matrix->leds[i] = CRGB::Black;

		}
		_matrix->update();
		return;
	}
	byte r, g, b;
	for (byte column = 1; column <= 16; column++)
	{
		for (byte row = 1; row <= 16; row++)
		{
			b = _bmpFile.read();
			g = _bmpFile.read();
			r = _bmpFile.read();
			CRGB color = CRGB(r, g, b);
			_matrix->setPixel(row, column, color);
		}
	}
	_matrix->update();
	_bmpFile.close();
	_processTime = millis() - _processTime;
	//Serial.println("Set all pixel to BMP file values");
}
/*
void BMPanim::processBMP(String s_path)
{
	s_path = s_path + ".bmp";
	//Serial.println("BMPanim processBMP");
	_bmpFile = *_fileHelper->readBMP(s_path.c_str());
	if (!_bmpFile)
	{
		for (int i = 0; i < numLEDs; i++)
		{
			_matrix->leds[i] = CRGB::Black;

		}
		_matrix->update();
	}
	byte r, g, b;
	for (byte column = 1; column <= 16; column++)
	{
		for (byte row = 1; row <= 16; row++)
		{
			b = _bmpFile.read();
			g = _bmpFile.read();
			r = _bmpFile.read();
			CRGB color = CRGB(r, g, b);
			_matrix->setPixel(row, column, color);
		}
	}
	_matrix->update();
	_bmpFile.close();
	//Serial.println("Set all pixel to BMP file values");
}
*/
void BMPanim::getInfo(String s_path)
{
	Serial.print("Opening INI file: ");
	String path = s_path + "config.ini";
	Serial.println(path);
	_fileHelper->readConfig(path.c_str());
	animation._hold = _fileHelper->cfg._hold;
	animation._loopAnim = _fileHelper->cfg._loopAnim;
	animation._loopMove = _fileHelper->cfg._loopMove;
	animation._moveX = _fileHelper->cfg._moveX;
	animation._moveY = _fileHelper->cfg._moveY;
	animation._nextfolder = _fileHelper->cfg._nextfolder;
	animation._panoff = _fileHelper->cfg._panoff;
	animation._frames = _fileHelper->cfg._frames;
}

void BMPanim::processFolder(const char * cc_path)
{
	getInfo(String(cc_path)+"/");
	if (animation._frames == 0)
	{
		Serial.println("Indexing folder");
		animation._frames = _fileHelper->indexFolder(cc_path);
	}
	Serial.println("Processing folder");
	breakout = false;
	_startanimTime = millis();
	do
	{
		if (millis() - _startanimTime + _processTime >= ANIMATION_DURATION)
		{
			processBMP((String(cc_path) + "/0.bmp").c_str());
			breakout = true;
		}
		for (int i = 0; i < animation._frames; i++)
		{
			//TODO [FIX] break animation when Image is not processed correctly
			processBMP((String(cc_path) + "/" + i +".bmp").c_str());
			delay(animation._hold);
		}
	} while (animation._loopAnim && !breakout);
	Serial.println("Breakout condidtion reached");
}

