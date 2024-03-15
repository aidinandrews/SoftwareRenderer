#pragma once
#define NOMINMAX
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "SDL.h"
#include "color.h"
#include "EasyBMP.h"
#include "vec2.h"
#include "line.h"

struct Palette {
public:
	const static int PALETTE_TYPE_6_BIT_RGB = 0;
private:
	Uint32* colors;
	int paletteSize;
public:
	Palette(const int paletteType);
	~Palette() { delete[] colors; }
	int getSize() { return paletteSize; }
	int findClosestColorIndex(Uint32 inputColor);
	Uint32 getColor(int index) { return colors[index]; }
};

class Uint32BMP {
private:
	Uint32* p_image;
	int* p_paletteIndex;
	int m_width, m_height;
	point2 m_size;

	Uint32(Uint32BMP::*p_getPixel)(int, int);
	Palette* p_palette;
public:
	Uint32BMP();
	Uint32BMP(const char* path);
	Uint32BMP(const char* path, Palette* palette);
	~Uint32BMP();

	void init(const char* path);
	Uint32 getPixel(int x, int y);

	Uint32 getPixelDirect(int x, int y);
	Uint32 getPixelFromPalette(int x, int y);

	int getWidth();
	int getHeight();
	point2 getSize();
};