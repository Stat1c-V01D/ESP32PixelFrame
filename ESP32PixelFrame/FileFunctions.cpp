#include "FileFunctions.h"

FileFunctions::FileFunctions()
{
	Serial.println("FileFunctions constructed");
	init();
}

FileFunctions::~FileFunctions()
{
	_file.close();
	SD.end();
}

void FileFunctions::init()
{
	Serial.println("FileFunctions init");
	initSD();
}

void FileFunctions::initSD()
{
	Serial.println("FileFunctions initSD");
	if (!SD.begin(SD_PIN)) {
		Serial.println("Card Mount Failed or missing SD Card");
		//matrix.showError();
		while (!SD.begin(SD_PIN))
		{
			SD.end();
			delay(1000);
			Serial.println("Remount Failed");
		}
	}
	Serial.println("Mount successful!");
	File root = SD.open("/");
	Serial.println("Root directory set");
	indexSD(root);
	root.close();
}

void FileFunctions::indexSD(File& dir)
{
	//create fileindex.ini in /00system folder with  section [0 to foldercount of first folder on SD] with key "sdpath" and value "full path to animation folder"
	//delete and recreate this file on function call <-- currently disabled
	//for every new animation read the ini for full animation folder path
	Serial.println("Indexing SD structure");
	File config;
	if (SD.exists("/00system/fileindex.ini"))
	{
		//SD.remove("/00system/fileindex.ini"); //TODO [FIX] too many writes to SD card on the long shot
		return;
	}
	config = SD.open("/00system/fileindex.ini", FILE_WRITE);
	config.println("#This files contains all path information to animations present on SD card");
	while (true)
	{
		File entry = dir.openNextFile();
		if (!entry)
		{
			// no more files
			break;
		}
		if (entry.isDirectory()) {
			config.print("[animation");
			config.print(foldercount);
			config.println("]");
			config.print("sdpath = ");
			config.println(entry.name());
			config.println();
			foldercount++;
		}
		entry.close();
	}
	config.println("[folders]");
	config.print("count = ");
	config.println(foldercount);
	config.close();
}


int FileFunctions::indexFolder(const char* cc_folderpath)
{
	int counter = 0;
	File folder = SD.open(cc_folderpath);
	File file = folder.openNextFile();
	while (file)
	{
		String filename = file.name();
		//Serial.println(filename);
		if (filename.indexOf(".bmp") != -1 || filename.indexOf(".BMP") != -1)
			counter++;
		file = folder.openNextFile();
	}
	folder.close();
	file.close();
	Serial.print("Files in Folder: ");
	Serial.println(counter);
	addFrameCount(cc_folderpath, counter);
	return counter;
}
/*
int FileFunctions::indexFolder(String s_folderpath)
{
	int counter = 0;
	File folder = SD.open(s_folderpath);
	File file = folder.openNextFile();
	while (file)
	{
		String filename = file.name();
		//Serial.println(filename);
		if (filename.indexOf(".bmp") != -1 || filename.indexOf(".BMP") != -1)
			counter++;
		file = folder.openNextFile();
	}
	folder.close();
	file.close();
	Serial.print("Files in Folder: ");
	Serial.println(counter);
	addFrameCount(s_folderpath.c_str(), counter);
	return counter;
}
*/
void FileFunctions::addFrameCount(const char* cc_folderpath, int i_counter)
{
	Serial.print("Adding framecount to config of: ");
	Serial.println(String(cc_folderpath) + "/config.ini");
	File configfile = SD.open(String(cc_folderpath) + "/config.ini", FILE_APPEND);
	//Serial.println(configfile.name());
	configfile.println();
	configfile.println("[frame]");
	configfile.println("framecount = " + String(i_counter));
	configfile.close();
}

File *FileFunctions::readBMP(const char* cc_filepath)
{
	//TODO [FIX] Debug levels
	if (DEBUG == 1) {
		Serial.print("loading File:  ");
		Serial.println(String(cc_filepath));
	}

	_file = SD.open(cc_filepath, FILE_READ);
	if (!_file) {
		Serial.println("Error: Failed to open");
	}
	struct bmp_file_header {
		uint16_t signature;
		uint32_t file_size;
		uint32_t reserved[4];
		uint32_t image_offset;
	};
	struct bmp_info_header {
		uint32_t header_size;
		uint32_t image_width;
		uint32_t image_height;
		uint16_t color_planes;
		uint16_t bits_per_pixel;
		uint32_t compression_method;
		uint32_t image_size;
		uint32_t horizontal_resolution;
		uint32_t vertical_resolution;
		uint32_t colors_in_palette;
		uint32_t important_colors;
	};
	bmp_file_header file_header;
	bmp_info_header info_header;
	file_header.signature = readNbytes16(_file);
	_file.seek(_file.position() + 8);
	file_header.image_offset = readNbytes32(_file);
	_file.seek(_file.position() + 4);
	info_header.image_width = readNbytes32(_file);
	info_header.image_height = readNbytes32(_file);
	_file.seek(_file.position() + 2);
	info_header.bits_per_pixel = readNbytes16(_file);
	info_header.compression_method = readNbytes32(_file);
	//TODO [QOL] Error handling
	if (file_header.signature != 0x4D42) //BMP signature check
	{
		Serial.print("Error: Invalid file signature!");
		Serial.println(file_header.signature);
		_file.close();
		return &_file;
	}
	if (info_header.compression_method != 0) //check for no compression
	{
		Serial.print("Error: Invalid compression!");
		Serial.println(info_header.compression_method); 
		_file.close();
		return &_file;
	}
	if (info_header.image_width != 16 || info_header.image_height != 16) //check that image is 16x16
	{
		Serial.print("Error: Invalid resolution!");
		Serial.println(info_header.image_width + "x" + info_header.image_height);
		_file.close();
		return &_file;
	}
	if (info_header.bits_per_pixel != 24) //check for 24 bit BMP
	{
		Serial.println("Error: Only 24 bit files are supported!");
		_file.close();
		return &_file;
	}
	_file.seek(file_header.image_offset);   //skip the whole header
	//Serial.println("Returning BMP File");
	return &_file;
}

/*
File * FileFunctions::readBMP(String s_filepath)
{
	if (DEBUG) {
		Serial.print("loading File:  ");
		Serial.println(String(s_filepath));
	}
	_file = SD.open(s_filepath, FILE_READ);
	if (!_file) {
		Serial.println("Error: Failed to open");
	}
	struct bmp_file_header {
		uint16_t signature;
		uint32_t file_size;
		uint32_t reserved[4];
		uint32_t image_offset;
	};
	struct bmp_info_header {
		uint32_t header_size;
		uint32_t image_width;
		uint32_t image_height;
		uint16_t color_planes;
		uint16_t bits_per_pixel;
		uint32_t compression_method;
		uint32_t image_size;
		uint32_t horizontal_resolution;
		uint32_t vertical_resolution;
		uint32_t colors_in_palette;
		uint32_t important_colors;
	};
	bmp_file_header file_header;
	bmp_info_header info_header;
	file_header.signature = readNbytes16(_file);
	_file.seek(_file.position() + 8);
	file_header.image_offset = readNbytes32(_file);
	_file.seek(_file.position() + 4);
	info_header.image_width = readNbytes32(_file);
	info_header.image_height = readNbytes32(_file);
	_file.seek(_file.position() + 2);
	info_header.bits_per_pixel = readNbytes16(_file);
	info_header.compression_method = readNbytes32(_file);

	if (file_header.signature != 0x4D42) //BMP signature check
	{
		Serial.print("Error: Invalid file signature!");
		Serial.println(file_header.signature);
	}
	if (info_header.compression_method != 0) //check for no compression
	{
		Serial.print("Error: Invalid compression!");
		Serial.println(info_header.compression_method);
	}
	if (info_header.image_width != 16 || info_header.image_height != 16) //check that image is 16x16
	{
		Serial.print("Error: Invalid resolution!");
		Serial.println(info_header.image_width + "x" + info_header.image_height);
	}
	if (info_header.bits_per_pixel != 24) //check for 24 bit BMP
	{
		Serial.println("Error: Only 24 bit files are supported!");
	}
	_file.seek(file_header.image_offset);   //skip the whole header
	//Serial.println("Returning BMP File");
	return &_file;
}
*/
void FileFunctions::readConfig(const char * cc_filepath)
{
	IniFile _ini(cc_filepath);
	Serial.println("INIfile class ready");
	if (!_ini.open())
	{
		Serial.print(F("Ini file "));
		Serial.print(cc_filepath);
		Serial.println(F(" does not exist"));
		// Cannot do anything else
		while (1)
			; //TODO [QOL] Error Handling
	}
	Serial.print(F("Ini file exists :"));
	Serial.println(cc_filepath);
	if (!_ini.validate(_buffer, _bufferLen)) {
		Serial.print("ini file ");
		Serial.print(_ini.getFilename());
		Serial.print(" not valid: ");
		printErrorMessage(_ini.getError());
		// Cannot do anything else
		while (1)
			;//TODO [QOL] Error Handling
	}
	Serial.println("Reading Animation Config");
	String tmp;
	if (_ini.getValue("animation", "hold", _buffer, _bufferLen))
	{
		cfg._hold = String(_buffer).toInt();
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("animation");
		Serial.print("Key: ");
		Serial.println("hold");
		printErrorMessage(_ini.getError());
		cfg._hold = 100;
	}
	if (_ini.getValue("animation", "loop", _buffer, _bufferLen))
	{
		tmp = String(_buffer);
		if (tmp == "true")
		{
			cfg._loopAnim = true;
		}
		else cfg._loopAnim = false;
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("animation");
		Serial.print("Key: ");
		Serial.println("loop");
		printErrorMessage(_ini.getError());
		cfg._loopAnim = false;
	}
	if (_ini.getValue("translate", "moveX", _buffer, _bufferLen))
	{
		cfg._moveX = String(_buffer).toInt();
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("translate");
		Serial.print("Key: ");
		Serial.println("moveX");
		printErrorMessage(_ini.getError());
		cfg._moveX = 0;
	}
	if (_ini.getValue("translate", "moveY", _buffer, _bufferLen))
	{
		cfg._moveY = String(_buffer).toInt();
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("translate");
		Serial.print("Key: ");
		Serial.println("moveY");
		printErrorMessage(_ini.getError());
		cfg._moveY = 0;
	}
	if (_ini.getValue("translate", "loop", _buffer, _bufferLen))
	{
		tmp = String(_buffer);
		if (tmp == "true")
		{
			cfg._loopMove = true;
		}
		else cfg._loopMove = false;
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("translate");
		Serial.print("Key: ");
		Serial.println("loop");
		printErrorMessage(_ini.getError());
		cfg._loopMove = false;
	}
	if (_ini.getValue("translate", "panoff", _buffer, _bufferLen))
	{
		tmp = String(_buffer);
		if (tmp == "true")
		{
			cfg._panoff = true;
		}
		else cfg._panoff = false;
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("translate");
		Serial.print("Key: ");
		Serial.println("panoff");
		printErrorMessage(_ini.getError());
		cfg._panoff = false;
	}
	if (_ini.getValue("translate", "nextFolder", _buffer, _bufferLen))
	{
		cfg._nextfolder = String(_buffer);
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("translate");
		Serial.print("Key: ");
		Serial.println("nextFolder");
		printErrorMessage(_ini.getError());
		cfg._nextfolder = "";
	}
	if (_ini.getValue("frame", "framecount", _buffer, _bufferLen))
	{
		cfg._frames = String(_buffer).toInt();
	}
	else
	{
		Serial.print("Section: ");
		Serial.println("frame");
		Serial.print("Key: ");
		Serial.println("framecount");
		printErrorMessage(_ini.getError());
		cfg._frames = 0;
	}
	_ini.close();
}

String FileFunctions::getFolderpath(int i_index, const char * cc_filepath)
{
	IniFile _ini(cc_filepath);
	Serial.println("INIfile class ready");
	if (!_ini.open())
	{
		Serial.print(F("Ini file "));
		Serial.print(cc_filepath);
		Serial.println(F(" does not exist"));
		// Cannot do anything else
		while (1)
			; //TODO [QOL] Error Handling
	}
	Serial.print(F("Ini file exists: "));
	Serial.println(cc_filepath);
	if (!_ini.validate(_buffer, _bufferLen)) {
		Serial.print("ini file ");
		Serial.print(_ini.getFilename());
		Serial.print(" not valid: ");
		printErrorMessage(_ini.getError());
		// Cannot do anything else
		while (1)
			;//TODO [QOL] Error Handling
	}
	Serial.println("Reading Folder Config");
	String s_section = "animation" + String(i_index);
	Serial.print("Searching for Section: ");
	Serial.println(s_section.c_str());
	const char * cc2 = "sdpath";
	if (_ini.getValue(s_section.c_str(), cc2, _buffer, _bufferLen))
	{
		Serial.print("AnimFolder: ");
		Serial.println(_buffer);
		_ini.close();
		return String(_buffer);
	}
	else
	{
		Serial.print("Section: ");
		Serial.println(s_section);
		Serial.print("Key: ");
		Serial.println(cc2);
		printErrorMessage(_ini.getError());
		_ini.close();
		return "/00system"; //TODO [FIX] Error handling
	}
}

int FileFunctions::getFoldercount(const char * cc_filepath)
{
	IniFile _ini(cc_filepath);
	Serial.println("INIfile class ready");
	if (!_ini.open())
	{
		Serial.print(F("Ini file "));
		Serial.print(cc_filepath);
		Serial.println(F(" does not exist"));
		// Cannot do anything else
		while (1)
			; //TODO [QOL] Error Handling
	}
	Serial.print(F("Ini file exists: "));
	Serial.println(cc_filepath);
	if (!_ini.validate(_buffer, _bufferLen)) {
		Serial.print("ini file ");
		Serial.print(_ini.getFilename());
		Serial.print(" not valid: ");
		printErrorMessage(_ini.getError());
		// Cannot do anything else
		while (1)
			;//TODO [QOL] Error Handling
	}
	Serial.println("Reading Foldercount");
	const char * cc1 = "folders";
	const char * cc2 = "count";
	if (_ini.getValue(cc1, cc2, _buffer, _bufferLen))
	{
		_ini.close();
		return String(_buffer).toInt();
	}
	else
	{
		Serial.print("Section: ");
		Serial.println(cc1);
		Serial.print("Key: ");
		Serial.println(cc2);
		printErrorMessage(_ini.getError());
		_ini.close();
		return 0;
	}
}



void FileFunctions::printErrorMessage(uint8_t e, bool eol)
{
	switch (e) {
	case IniFile::errorNoError:
		Serial.print(F("no error"));
		break;
	case IniFile::errorFileNotFound:
		Serial.print(F("file not found"));
		break;
	case IniFile::errorFileNotOpen:
		Serial.print(F("file not open"));
		break;
	case IniFile::errorBufferTooSmall:
		Serial.print(F("buffer too small"));
		break;
	case IniFile::errorSeekError:
		Serial.print(F("seek error"));
		break;
	case IniFile::errorSectionNotFound:
		Serial.print(F("section not found"));
		break;
	case IniFile::errorKeyNotFound:
		Serial.print(F("key not found"));
		break;
	case IniFile::errorEndOfFile:
		Serial.print(F("end of file"));
		break;
	case IniFile::errorUnknownError:
		Serial.print(F("unknown error"));
		break;
	default:
		Serial.print(F("unknown error value"));
		break;
	}
	if (eol)
		Serial.println();
}

int32_t FileFunctions::readNbytes32(File & p_file, int pos, byte nBytes)
{
	if (pos == -1)
		pos = p_file.position();
	if (nBytes > 4)
		return 0;
	if (p_file.position() != pos)
		p_file.seek(pos);

	int32_t weight = 1;
	int32_t result = 0;
	for (; nBytes; nBytes--)
	{
		result += weight * p_file.read();
		weight <<= 8;
	}
	return result;
}

int16_t FileFunctions::readNbytes16(File & p_file, int pos, byte nBytes)
{
	if (pos == -1)
		pos = p_file.position();
	if (nBytes > 4)
		return 0;
	if (p_file.position() != pos)
		p_file.seek(pos);

	int16_t weight = 1;
	int16_t result = 0;
	for (; nBytes; nBytes--)
	{
		result += weight * p_file.read();
		weight <<= 8;
	}
	return result;
}
