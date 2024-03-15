#include "bmp.h"

// COLOR PALETTE
//############################################################################################

void create6BitPalette(Uint32*& palette) {
	palette = new Uint32[64];
	int paletteIndex = 0;
	Uint8 red, green, blue;
	Uint8 colorStep = 255 / 4;
	for (int r = 0; r < 4; r++) {
		red = r * colorStep;
		for (int g = 0; g < 4; g++) {
			green = g * colorStep;
			for (int b = 0; b < 4; b++) {
				blue = b * colorStep;
				palette[paletteIndex] = makeColor(red, green, blue, 255);
				paletteIndex++;
			}
		}
	}
}

Palette::Palette(const int paletteType) {
	switch (paletteType) {

	case PALETTE_TYPE_6_BIT_RGB:
		create6BitPalette(colors);
		paletteSize = 64;
		break;

	default:
		colors = nullptr;
		paletteSize = 0;
	}
}

int Palette::findClosestColorIndex(Uint32 inputColor) {
	float closestColorDist = FLT_MAX;
	float currentColorDist;
	int closestColorIndex;
	point3 inputColorPosition(getRed(inputColor), getGreen(inputColor), getBlue(inputColor));
	for (int i = 0; i < paletteSize; i++) {
		Uint32 currentColor = colors[i];
		point3 currentColorPosition(getRed(currentColor), getGreen(currentColor), getBlue(currentColor));
		currentColorDist = getLineDist3d(inputColorPosition, currentColorPosition);
		if (currentColorDist < closestColorDist) {
			currentColorDist = closestColorDist;
			closestColorIndex = i;
		}
	}
	return closestColorIndex;
}

// IMAGE
//############################################################################################

Uint32BMP::Uint32BMP() {
	p_image = new Uint32;
	*p_image = 0xFFFFFFFF;
	m_width = 1;
	m_height = 1;
}

Uint32BMP::Uint32BMP(const char* path) {
	init(path);
	p_getPixel = &Uint32BMP::getPixelDirect;
}

Uint32BMP::Uint32BMP(const char* path, Palette* palette) {
	init(path);
	int imageSize = m_width * m_height;
	p_paletteIndex = new int[imageSize];
	for (int i = 0; i < imageSize; i++) {
		p_paletteIndex[i] = palette->findClosestColorIndex(p_image[i]);
	}
	delete[] p_image;

	p_getPixel = &Uint32BMP::getPixelFromPalette;
}

void Uint32BMP::init(const char* path) {
	BMP image;
	image.ReadFromFile(path);

	m_width = image.TellWidth();
	m_height = image.TellHeight();
	p_image = new Uint32[m_width * m_height];
	m_size = point2(float(m_width), float(m_height));

	int pos = 0;
	Uint8 r = 0, g = 0, b = 0, a = 255;
	Uint32 color;
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			pos = y * m_width + x;
			r = image(x, y)->Red;
			g = image(x, y)->Green;
			b = image(x, y)->Blue;
			color = makeColor(r, g, b, a);
			p_image[pos] = color;
		}
	}
}

Uint32BMP::~Uint32BMP() {
	delete[] p_image;
	delete[] p_paletteIndex;
}

Uint32 Uint32BMP::getPixel(int x, int y) {
	// If the image uses a palette, the function pointer will grab a color from the palette	
	return p_image[std::min(std::max(0, y * m_width + x),100)];
	return (this->*p_getPixel)(x, y);
}
Uint32 Uint32BMP::getPixelDirect(int x, int y) {
	return p_image[x + y * m_width];
}
Uint32 Uint32BMP::getPixelFromPalette(int x, int y) {
	int index = p_paletteIndex[x + y * m_width];
	return p_palette->getColor(index);
}

int Uint32BMP::getWidth() {
	return m_width;
}

int Uint32BMP::getHeight() {
	return m_height;
}

point2 Uint32BMP::getSize() {
	return m_size;
}
