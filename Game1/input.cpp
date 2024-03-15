#include "input.h"

Input::Input()
	: _mouseX(0), _mouseY(0)
{}

void Input::getInputs(SDL_Event& event, const Uint8* state, Camera& camera, bool& quit) {

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) { quit = true; }
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			//check for button presses
			if (state[SDL_SCANCODE_W]) { camera.adjPos(0, -5, 0); }
			if (state[SDL_SCANCODE_A]) { camera.adjPos(-5, 0, 0); }
			if (state[SDL_SCANCODE_S]) { camera.adjPos(0, 5, 0); }
			if (state[SDL_SCANCODE_D]) { camera.adjPos(5, 0, 0); }
			if (state[SDL_SCANCODE_Q]) { camera.adjYaw(0.05f); }
			if (state[SDL_SCANCODE_E]) { camera.adjYaw(-0.05f); }
			if (state[SDL_SCANCODE_UP]) { camera.adjPitch(0.05f); }
			if (state[SDL_SCANCODE_DOWN]) { camera.adjPitch(-0.05f); }
		}
		//zoom in and out with mouse wheel
		if (event.type == SDL_MOUSEWHEEL) {
			if (event.wheel.y > 0) { camera.adjZoom(0.2f); }
			else if (event.wheel.y < 0) { camera.adjZoom(-0.2f); }
		}
		//store current mouse position
		if (event.type == SDL_MOUSEMOTION) { SDL_GetMouseState(&_mouseX, &_mouseY); }
		_mousePos.x() = float(_mouseX);
		_mousePos.y() = float(_mouseY);
		//adjust camera view based on click + drag mouse
		//store mouse position of the last place clicked
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			_mouseStore = _mousePos;
			if (event.button.button == (SDL_BUTTON_LEFT)) { leftMouseClicked = true; }
			if (event.button.button == (SDL_BUTTON_RIGHT)) { rightMouseClicked = true; }
		}
		if (leftMouseClicked || rightMouseClicked) {
			_mouseAdj = _mouseStore - _mousePos;
		}
		if (leftMouseClicked) {
			camera.m_yawAdj = float(_mouseAdj.x() * .005f);
			camera.m_pitchAdj = float(_mouseAdj.y() * .005f);
		}
		if (rightMouseClicked) {
			float x = -_mouseAdj.x() * (1 / camera.m_zoom);
			float y = -_mouseAdj.y() * (1 / camera.m_zoom);
			//adjust camera position while keeping in mind the camera rotation
			camera.m_deltaAdj.x() = x * cos(-camera.m_yaw) - y * sin(-camera.m_yaw);
			camera.m_deltaAdj.y() = y * cos(-camera.m_yaw) + x * sin(-camera.m_yaw);
		}
		//add adjustments to the camera based on how far the mouse moved
		//then reset the adjustments for a new click
		if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == (SDL_BUTTON_LEFT)) {
				leftMouseClicked = false;
				camera.m_yawStore += camera.m_yawAdj;
				camera.m_yawAdj = 0;
				camera.m_pitchStore += camera.m_pitchAdj;
				camera.m_pitchAdj = 0;

			}
			if (event.button.button == (SDL_BUTTON_RIGHT)) {
				rightMouseClicked = false;
				camera.m_deltaStore += camera.m_deltaAdj;
				camera.m_deltaAdj = 0;
			}
			_mouseAdj = 0;
		}
	}

}