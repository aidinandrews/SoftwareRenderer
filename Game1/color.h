#pragma once
#include "SDL.h"
#include <iostream>

Uint32 randColor();
Uint8 getRed(Uint32 color);
Uint8 getGreen(Uint32 color);
Uint8 getBlue(Uint32 color);
Uint8 getAlpha(Uint32 color);
Uint32 darkenColor(Uint32& color, float amount);
Uint32 makeColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
Uint32 blendColors(Uint32 A, Uint32 B);
Uint32 blendColors(Uint32 A, Uint32 B, float amount);