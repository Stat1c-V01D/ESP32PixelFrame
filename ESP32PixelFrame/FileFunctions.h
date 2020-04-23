#pragma once
#include "config.h"
#include <SD.h>
#include "IniFileLiteESP32.h"
class FileFunctions
{
public:
	FileFunctions();
	~FileFunctions();
	void init();
	void initSD();
	int foldercount = 0;
	void indexSD(File& dir);
	int indexFolder(const char* cc_folderpath);
	//int indexFolder(String s_folderpath);
	void addFrameCount(const char* c_folderpath, int i_counter);
	File *readBMP(const char* cc_filepath);
	//File *readBMP(String s_filepath);
	void readConfig(const char * cc_filepath);
	String getFolderpath(int i_index,const char * cc_filepath = "/00system/fileindex.ini");
	int getFoldercount(const char * cc_filepath = "/00system/fileindex.ini");
	struct animInfo
	{
		int _frames, _hold, _moveX, _moveY;
		bool _loopAnim, _loopMove, _panoff;
		String _nextfolder = "";
	}cfg;
	void printErrorMessage(uint8_t e, bool eol = true);
	int32_t readNbytes32(File& p_file, int pos = -1, byte nBytes = 4);
	int16_t readNbytes16(File& p_file, int pos = -1, byte nBytes = 2);
protected:
	File _file;
private:
	const size_t _bufferLen = 80;
	char _buffer[80];
};

