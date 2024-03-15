#include "game.h"
#include "bmp.h"
#include "EasyBMP.h"
#include "entity.h"
#include "objLoader.h"
#include "entity.h"
#include "plant.h"
#include "vertexData.h"
#include "renderer2.h"
#include <vector>
#include <thread>

Game::Game() : quit(false) {
	renderer.init(&screen, &camera);
	camera.p_screen = &screen;
}
Game::~Game() {}

void print(int& a) { for (int i = 0; i < 100; i++) { std::cout << a << std::endl; } }

void Game::run() {
	Renderer2 renderer2;

	// declare and read the bitmap
	Uint32BMP texture("bitmaps\\wave.bmp");
	float textureWidth = (float)texture.getWidth() - 1;
	float textureHeight = (float)texture.getHeight() - 1;

	if (screen.init() == false) {
		std::cout << "Error initializing SDL." << std::endl;
	}
	//test obj
	Entity teapot("objFiles\\newell_teaset\\teapot.obj", &renderer);
	teapot.p_renderer2 = &renderer2;
	teapot.p_textureMap = renderer2.p_defaultTexture;
	teapot.move(point3(0, 0, 200));
	teapot.scale(100);

	Entity sphere("objFiles\\sphere.txt", &renderer);
	sphere.p_renderer2 = &renderer2;
	sphere.p_textureMap = renderer2.p_defaultTexture;
	sphere.m_textureGood = false;
	sphere.scale(100);
	sphere.move(point3(200, 200, 200));

	Entity horse("objFiles\\horse.obj", &renderer);
	horse.p_renderer2 = &renderer2;
	horse.p_textureMap = renderer2.p_defaultTexture;
	horse.m_textureGood = false;
	horse.scale(100);
	horse.move(point3(200, 200, 200));

	Entity floor("objFiles\\floorTriangles.txt", &renderer);
	floor.p_renderer2 = &renderer2;
	floor.p_textureMap = renderer2.p_defaultTexture;
	floor.scale(500);
	floor.m_textureGood = true;
	floor.m_backFaceCulling = false;

	std::vector<Plant> plants;
	for (int j = 0; j < 4; j++) {
		Plant plant(&renderer);
		plants.push_back(plant);
		for (int i = 0; i < 50; i++) {
			plants[j].addBranch();
		}
	}
	plants[1].teleport(point3(500, 0, 0));
	plants[2].teleport(point3(0, 500, 0));
	plants[3].teleport(point3(500, 500, 0));
	
	renderer2.init(&screen, &camera);

	//game loop!!
	while (!quit) {
		//Update Screen
		screen.update();
		screen.clear();
		user.getInputs(event, state, camera, quit);
		camera.update();
		float timeStart = float(SDL_GetTicks());

		if (state[SDL_SCANCODE_R]) {
			teapot.rotate(point3(0.0005f, 0.001f, 0.002f));
			//sphere.rotate(point3((1.0f * rand() / RAND_MAX * 0.05f), (1.0f * rand() / RAND_MAX * 0.05f), (1.0f * rand() / RAND_MAX * 0.05f)));
		}
		sphere.render2();
		
		//floor.drawShadow();
		teapot.drawShadow();
		//horse.render2();
		floor.render2();
		teapot.render2();
		
		for (int i = 0; i < plants.size(); i++) {
			//plants[i].render();
		}

		float elapsed = float(SDL_GetTicks());
		float sleepTime = 16 - (elapsed - timeStart);
		if (sleepTime < 0) { sleepTime = 0; }
		float frameTime = float(SDL_GetTicks());
		Sleep(DWORD(sleepTime));
		elapsed = float(SDL_GetTicks());

		if (state[SDL_SCANCODE_F]) {
			int minFrameTime = int(1000 / (frameTime - timeStart));
			int fps = int(1000.0f / (elapsed - timeStart));
			if (fps < 0) { fps = 1000; }
			std::cout << "real: " << fps << "fps\nmax: " << minFrameTime << "fps" << std::endl << std::endl;
		}
		// Show shadow map:
		if (state[SDL_SCANCODE_V]) {
			for (int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
				for (int x = 0; x < Screen::SCREEN_WIDTH; x++) {

					float s = screen.m_shadowMap[x + y * Screen::SCREEN_WIDTH];
					Uint8 value = Uint8(s / 500 * 255 * -1 + 255);
					Uint32 color = makeColor(value, value, value, 255);
					screen.setPixel(x, y, color);
					//std::cout << s << std::endl;
				}
			}
		}
		// Show Z buffer:
		if (state[SDL_SCANCODE_C]) {
			Uint8 r = 0, g = 0, b = 0, a = 255, v = 0;
			for (int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
				for (int x = 0; x < Screen::SCREEN_WIDTH; x++) {

					float z = screen.m_zBuffer[x + y * Screen::SCREEN_WIDTH];
					v = Uint8(sin(z / 5000 * M_PI) * 255);

					Uint32 color = makeColor(v, v, v, 255);
					screen.setPixel(x, y, color);
				}
			}
		}
		//zoom in
		if (!state[SDL_SCANCODE_M]) {
			int sizeOfZoom = 100 * Screen::m_superSampling / Screen::m_pixelization;
			int topLeftY = screenHeight / 2;
			int topLeftX = screenWidth / 2;
			int magX = 0, magY = 0;
			for (int y = 0; y < sizeOfZoom; y++) {
				for (int x = 0; x < sizeOfZoom; x++) {
					Uint32 color = screen.getPixel(x + topLeftX, y + topLeftY);
					//screen.setPixel(x, y, color);
					screen.setPixel(magX, magY, color);
					screen.setPixel(magX + 1, magY, color);
					screen.setPixel(magX, magY + 1, color);
					screen.setPixel(magX + 1, magY + 1, color);
					magX += 2;
				}
				magX = 0;
				magY += 2;
			}
		}
		// Dithering
		if (state[SDL_SCANCODE_I]) {
			// Convert to greyscale
			for (int i = 0; i < screenWidth * screenHeight; i++) {
				Uint32 color = screen.m_pixelBuffer[i];
				Uint8 r = getRed(color);
				Uint8 g = getGreen(color);
				Uint8 b = getBlue(color);
				Uint8 v = 0.11111111 * (((g + (g << 1) + r) << 1) + b);
				screen.m_pixelBuffer[i] = makeColor(v, v, v, 255);
			}
			// Floyd Steinberg Dithering
			/*for (int y = 0; y < screenHeight - 1; y++) {
				for (int x = 1; x < screenWidth - 1; x++) {
					int pos = x + screenWidth * y;
					int error = getRed(screen.m_pixelBuffer[pos]) - 150;

					if (int(error) > 0) {
						screen.m_pixelBuffer[pos] = 0xFFFFFFFF;
					}
					else {
						screen.m_pixelBuffer[pos] = 0x000000FF;
					}
					Uint8 mr = (7.0f / 16.0f) * float(error) + getRed(screen.m_pixelBuffer[pos + 1]);
					Uint8 bl = (3.0f / 16.0f) * float(error) + getRed(screen.m_pixelBuffer[pos - 1 + screenWidth]);
					Uint8 bm = (5.0f / 16.0f) * float(error) + getRed(screen.m_pixelBuffer[pos + screenWidth]);
					Uint8 br = (1.0f / 16.0f) * float(error) + getRed(screen.m_pixelBuffer[pos + 1 + screenWidth]);
					screen.m_pixelBuffer[pos + 1] = makeColor(mr, mr, mr, 255);
					screen.m_pixelBuffer[pos - 1 + screenWidth] = makeColor(bl, bl, bl, 255);
					screen.m_pixelBuffer[pos + screenWidth] = makeColor(bm, bm, bm, 255);
					screen.m_pixelBuffer[pos + 1 + screenWidth] = makeColor(br, br, br, 255);
				}
			}*/
			// Random Dithering
			/*for (int i = 0; i < screenWidth * screenHeight; i++) {
				Uint32 color = screen.m_pixelBuffer[i];
				Uint8 v = getRed(color);
				int randV = float(rand()) / RAND_MAX * 255;
				if (randV > int(v)) {
					screen.m_pixelBuffer[i] = 0x000000FF;
				}
				else {
					screen.m_pixelBuffer[i] = 0xFFFFFFFF;
				}
			}*/
			// Ordered Dithering
			float thresholdMap[4][4] =
			{
				{	0,			0.5f,		0.125f,		0.625f		},
				{	0.75f,		0.25f,		0.875f,		0.375f		},
				{	0.1875,		0.6876f,	0.0625f,	0.5625f		},
				{	0.9375f,	0.4375f,	0.8125f,	0.3125f		}
			};
			for (int y = 0; y < screenHeight; y++) {
				for (int x = 0; x < screenWidth; x++) {
					int pos = x + screenWidth * y;
					float v = getRed(screen.m_pixelBuffer[pos]);
					float threshold = thresholdMap[x % 4][y % 4] * 255;
					if (v - threshold > 0) {
						screen.m_pixelBuffer[pos] = 0xFFFFFFFF;
					}
					else {
						screen.m_pixelBuffer[pos] = 0;
					}

				}
			}
		}
	}
	screen.close();
}