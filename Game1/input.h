#pragma once
#include <iostream>
#include "vec2.h"
#include "SDL.h"
#include "camera.h"

class Input {
public:
	int _mouseX, _mouseY;
	point3 _mousePos;
	point3 _mouseStore;
	point3 _mouseAdj;
	bool rightMouseClicked = false;
	bool leftMouseClicked = false;
public:
	Input();
	//~Input();
	void getInputs(SDL_Event& event, const Uint8* state, Camera& camera, bool& quit);
};



