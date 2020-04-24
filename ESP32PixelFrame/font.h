#pragma once
#include "Matrix.h"
//currently PROGMEM in class members doesnt work on ESP8266
#ifdef ESP8266
#define PROG_MEM
#else
#define PROG_MEM PROGMEM
#endif

//TODO [CODE] built for custom fonts
class font
{
public:
	font(Matrix * pmatrix, int i_width = 3, int i_height = 5);
	~font();
	void drawNumber(byte s_x, byte s_y, String s, CRGB c1 = CRGB::White, CRGB c2 = CRGB::Black);
private:
	Matrix *_matrix;
	int fontwidth , fontheight ;
	const bool n1[15] PROG_MEM = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1 };
	const bool n2[15] PROG_MEM = { 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1 };
	const bool n3[15] PROG_MEM = { 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1 };
	const bool n4[15] PROG_MEM = { 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1 };
	const bool n5[15] PROG_MEM = { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 };
	const bool n6[15] PROG_MEM = { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 };
	const bool n7[15] PROG_MEM = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1 };
	const bool n8[15] PROG_MEM = { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 };
	const bool n9[15] PROG_MEM = { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 };
	const bool n0[15] PROG_MEM = { 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1 };
	const bool nc[15] PROG_MEM = { 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0 };
};

