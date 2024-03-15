#include "color.h"

Uint32 randColor() {
	Uint32 color = 0;
	color += Uint32(1.0 * rand() / RAND_MAX * 255);
	color <<= 8;
	color += Uint32(1.0 * rand() / RAND_MAX * 255);
	color <<= 8;
	color += Uint32(1.0 * rand() / RAND_MAX * 25);;
	color <<= 8;
	color += 0xFF;
	return color;
}

Uint8 getRed(Uint32 color) {
	return Uint8(color >>= 24);
}

Uint8 getGreen(Uint32 color) {
	return Uint8(color >>= 16);
}

Uint8 getBlue(Uint32 color) {
	return Uint8(color >>= 8);
}
Uint8 getAlpha(Uint32 color) {
	return Uint8(color);
}

Uint32 makeColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {

	Uint32 color = 0;
	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	return color;
}

Uint32 darkenColor(Uint32& color, float amount) {

	Uint8 alpha = color;
	Uint8 blue = Uint8(color >> 8) * amount;
	Uint8 green = Uint8(color >> 16) * amount;
	Uint8 red = Uint8(color >> 24) * amount;
	color = alpha | (blue << 8) | (green << 16) | (red << 24);
	return color;
}

Uint32 blendColors(Uint32 A, Uint32 B) {
	Uint8 red = (getRed(A) + getRed(B)) / 2;
	Uint8 green = (getGreen(A) + getGreen(B)) / 2;
	Uint8 blue = (getBlue(A) + getBlue(B)) / 2;
	return makeColor(red, green, blue, 255);
}

Uint32 blendColors(Uint32 A, Uint32 B, float amount) {
	float amountA = amount;
	float amountB = 1 - amount;
	int red = (getRed(A) * amountA) + (getRed(B) * amountB);
	int green = (getGreen(A) * amountA) + (getGreen(B) * amountB);
	int blue = (getBlue(A) * amountA) + (getBlue(B) * amountB);
	return makeColor(Uint8(red), Uint8(green), Uint8(blue), 255);
}