#pragma once
#include "SDL.h"
#include <iostream>
#include "vec2.h"
#include "color.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include <thread>

#define screenHeight Screen::SCREEN_HEIGHT
#define screenWidth Screen::SCREEN_WIDTH

class Screen
{
public:
	const static int m_superSampling = 1;
	const static int m_pixelization = 1;
	const static int SCREEN_WIDTH = (800 * m_superSampling) / m_pixelization;
	const static int SCREEN_HEIGHT = (600 * m_superSampling) / m_pixelization;
	const point3 screenSize = point3(float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0);
	const point3 screenCenter = screenSize / 2;
	const static int _bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT;
	const Uint8* state = SDL_GetKeyboardState(NULL);
public:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;

	Uint32* m_pixelBuffer;
	float* m_zBuffer;
	float* m_shadowMap;
public:
	Screen();
	bool init();
	void update();
	void setPixel(int x, int y, Uint32 color);
	Uint32 getPixel(int x, int y);
	void setZBuffer(int x, int y, float value);
	float getZBuffer(int x, int y);
	void setShadowMap(int x, int y, float value);
	float getShadowMap(int x, int y);
	void setGeometryBuffer(int x, int y, bool value);
	bool getGeometryBuffer(int x, int y);
	void setShadowBuffer(int x, int y, float value);
	float getShadowBuffer(int x, int y);
	bool processEvents();
	void clear();
	void close();

};