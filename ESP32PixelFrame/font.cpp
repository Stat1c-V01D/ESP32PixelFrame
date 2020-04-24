#include "font.h"

font::font(Matrix * pmatrix, int i_width, int i_height)
{
	_matrix = pmatrix;
	fontwidth = i_width;
	fontheight = i_height;
}

font::~font()
{
}

void font::drawNumber(byte s_x, byte s_y, String s, CRGB c1, CRGB c2)
{
	bool * numberTemp; //temporary non const pointer to store the right address
	for (byte i = 0; i < s.length(); i++) //go through the input string s f.e. "10"
	{
		byte x = s_x + (fontwidth + 1) * i; //x,y drawing position  for the current number
		byte y = s_y;
		switch (s[i]) //chose the right array for the current nuber
		{
		case '1': numberTemp = (bool*)n1;
			break;
		case '2': numberTemp = (bool*)n2;
			break;
		case '3': numberTemp = (bool*)n3;
			break;
		case '4': numberTemp = (bool*)n4;
			break;
		case '5': numberTemp = (bool*)n5;
			break;
		case '6': numberTemp = (bool*)n6;
			break;
		case '7': numberTemp = (bool*)n7;
			break;
		case '8': numberTemp = (bool*)n8;
			break;
		case '9': numberTemp = (bool*)n9;
			break;
		case 'c': numberTemp = (bool*)nc;
			break;
		default: numberTemp = (bool*)n0;
			break;
		}
		const bool* number = numberTemp; //make it const again
		for (byte column = 1, counter = 0; column <= fontheight; column++) //Set the right pixels in the led matrix based on the array chosen above.
		{
			for (byte row = 1; row <= fontwidth; row++, counter++)
			{
				if (number[counter] == true)
					_matrix->setPixel(x + row - 1, y + column - 1, c1);
				else
					_matrix->setPixel(x + row - 1, y + column - 1, c2);
			}
		}
	}
	_matrix->update();
}
