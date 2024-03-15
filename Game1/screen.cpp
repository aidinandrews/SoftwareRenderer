#pragma once
#include "screen.h"

//initializer
Screen::Screen() :
	m_window(NULL), m_renderer(NULL), m_texture(NULL),
	m_pixelBuffer(NULL), m_zBuffer(NULL), m_shadowMap(NULL) {
}

//initialize the window
bool Screen::init() {

	//check if the SDL library works good
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	//create window baybee
	m_window = SDL_CreateWindow("Lies", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH / m_superSampling * m_pixelization, SCREEN_HEIGHT / m_superSampling * m_pixelization, SDL_WINDOW_SHOWN);

	if (m_window == NULL) {
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
		SCREEN_WIDTH / m_superSampling * m_pixelization, SCREEN_HEIGHT / m_superSampling * m_pixelization);

	//check if the renderer and texture have been creates properly
	//if they havent, instead of crashing when some other part of the program tries to use them, the program will end
	if (m_renderer == NULL) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	if (m_texture == NULL) {
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}
	//allocate screen pixel buffers
	m_pixelBuffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_zBuffer = new float[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_shadowMap = new float[SCREEN_WIDTH * SCREEN_HEIGHT];
	//set the buffers to blacl pixels to start
	memset(m_pixelBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_zBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(float));
	memset(m_shadowMap, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(float));
	return true;
}

void Screen::setPixel(int x, int y, Uint32 color) {
	int pos = x + SCREEN_WIDTH * y;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return; }
	m_pixelBuffer[pos] = color;
}
Uint32 Screen::getPixel(int x, int y) {
	int pos = y * SCREEN_WIDTH + x;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return 0; }
	return m_pixelBuffer[pos];
}
void Screen::setZBuffer(int x, int y, float value) {
	int v = y * SCREEN_WIDTH + x;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return; }
	m_zBuffer[v] = value;
}
float Screen::getZBuffer(int x, int y) {
	int v = y * SCREEN_WIDTH + x;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return 1000000; }
	return m_zBuffer[v];
}
void Screen::setShadowMap(int x, int y, float value) {
	int v = y * SCREEN_WIDTH + x;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return; }
	m_shadowMap[v] = value;
}
float Screen::getShadowMap(int x, int y) {
	int v = y * SCREEN_WIDTH + x;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) { return 1000000; }
	return m_shadowMap[v];
}

void Screen::update() {
	if (m_superSampling > 1) {
		int red = 0, green = 0, blue = 0;
		Uint32 color = 0;
		int kernalSize = m_superSampling;
		int divide = kernalSize * kernalSize;
		for (int y = 0; y < screenHeight / kernalSize; y++) {
			for (int x = 0; x < screenWidth / kernalSize; x++) {

				int metaPos = (x * kernalSize) + (y * kernalSize * screenWidth);
				red = 0, green = 0, blue = 0;
				for (int ky = 0; ky < kernalSize; ky++) {
					for (int kx = 0; kx < kernalSize; kx++) {

						int pos = (x * kernalSize) + kx + (((y * kernalSize) + ky) * screenWidth);

						red += getRed(m_pixelBuffer[pos]);
						green += getGreen(m_pixelBuffer[pos]);
						blue += getBlue(m_pixelBuffer[pos]);
					}
				}

				red /= divide;
				green /= divide;
				blue /= divide;
				color = makeColor(red, green, blue, 255);
				m_pixelBuffer[(x)+((y)*screenWidth)] = color;
			}
		}
	}
	if (m_pixelization > 1) {
		Uint32* newPixels = new Uint32[screenWidth * m_pixelization * screenHeight * m_pixelization];
		memset(newPixels, 0, SCREEN_WIDTH * m_pixelization * m_pixelization * SCREEN_HEIGHT * sizeof(Uint32));

		int sw = screenWidth * m_pixelization;
		int sh = screenHeight * m_pixelization;

		for (int y = 0; y < screenHeight; y++) {
			for (int x = 0; x < screenWidth; x++) {

				
				//std::cout << x << "  " << y << "\n";

				for (int i = 0; i < m_pixelization; i++) {
					for (int j = 0; j < m_pixelization; j++) {

						int pos = x + y * screenWidth;
						int POS = (x * m_pixelization + j) + (y * m_pixelization + i) * sw;
						newPixels[POS] = m_pixelBuffer[pos];
					}
				}
			}
		}
		SDL_UpdateTexture(m_texture, NULL, newPixels, sw * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
		delete[] newPixels;
		return;
	}
	//copy info from buffer to the window
	SDL_UpdateTexture(m_texture, NULL, m_pixelBuffer, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

bool Screen::processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//check if the window was closed by clicking the x top left
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void Screen::clear() {
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		m_pixelBuffer[i] = 0x202080FF;
		//m_pixelBuffer[i] = 0xFFFF00FF;
		m_zBuffer[i] = 1000000;
		m_shadowMap[i] = 1000000;
	}
}

void Screen::close() {
	//deallocate all the space used by SDL
	delete[] m_pixelBuffer;
	delete[] m_zBuffer;
	delete[] m_shadowMap;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}