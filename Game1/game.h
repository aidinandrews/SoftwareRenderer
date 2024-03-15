#pragma once
#include "headers.h"

class Game {
public:
	Screen screen;
	Camera camera;
	Renderer renderer;
	Input user;
	SDL_Event event;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	bool quit;
public:
	Game();
	~Game();

	void run();
};