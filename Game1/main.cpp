#include <iostream>
#include "SDL.h"
#include "headers.h"
#include "game.h"

int main(int argc, char* argv[]) {

	srand(int(time(NULL)));

	Game game;
	game.run();

	return 0;
}