#pragma once
#include <thread>
#include "screen.h"
#include "camera.h"
#include "vec2.h"
#include "vec3.h"
#include "color.h"
#include "vertexData.h"
#include "bmp.h"
#include "line.h"

class Renderer2 {
private:
	Screen* p_screen;
	Camera* p_camera;
	vec3 m_lightDirection;
	Uint32 m_color;
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Used for triangle drawing.
	VertexData m_vertices[7];
	LineSegment m_AB, m_AC, m_BC;
	vec3 m_triangleNormal;
	LineSegment* p_leftSide, * p_rightSide;
	LineSegment m_scanline;
	int m_ySteps;
	bool m_backFaceCulling;
	int m_numPoints; // Used for drawing triangles cropped to the screen.
	// Various triangle types
	int m_triangleType;
	const int m_numThreads = 2;

public:
	Uint32BMP* p_texture;
	Uint32BMP* p_defaultTexture;
	const static int TRIANGLE_TYPE_SHADOW = -1;
	const static int TRIANGLE_TYPE_2D_FLAT_COLOR_NO_SHADING = 0;
	const static int TRIANGLE_TYPE_2D_TEXTURED_NO_SHADING = 1;
	const static int TRIANGLE_TYPE_3D_FLAT_COLOR_NO_SHADING = 2;
	const static int TRIANGLE_TYPE_3D_TEXTURED_NO_SHADING = 3;
	const static int TRIANGLE_TYPE_3D_FLAT_COLOR_FLAT_SHADING = 4;
	const static int TRIANGLE_TYPE_3D_TEXTURED_FLAT_SHADING = 5;
	const static int TRIANGLE_TYPE_3D_FLAT_COLOR_PHONG_SHADING = 6;
	const static int TRIANGLE_TYPE_3D_TEXTURED_PHONG_SHADING = 7;

public:
	Renderer2() : p_screen(nullptr), p_camera(nullptr), m_color(0), p_texture(nullptr), m_backFaceCulling(true), m_numPoints(3),
		p_leftSide(nullptr), p_rightSide(nullptr), m_ySteps(0), m_triangleType(0), m_lightDirection(vec3(0,0,1)) {

		Palette sixBit(Palette::PALETTE_TYPE_6_BIT_RGB);
		p_defaultTexture = new Uint32BMP("bitmaps\\wave.bmp", &sixBit);
	}
	~Renderer2() {
		delete p_defaultTexture;
	}

	int getTriangleType() {
		return m_triangleType;
	}

	Camera* getCamera() {
		return p_camera;
	}
	Screen* getScreen() {
		return p_screen;
	}

	void init(Screen* screen, Camera* camera) {
		p_screen = screen;
		p_camera = camera;
	}

	point2 project(point3 mapPosition, Camera& camera);

	void defineTriangleType(const int& triangleType);
	void renderTriangle(VertexData& A, VertexData& B, VertexData& C, Uint32& color, Uint32BMP* texture, vec3* normal);
	void renderTriangleShadow(VertexData& A, VertexData& B, VertexData& C, vec3* normal);
	void initTriangle(VertexData& A, VertexData& B, VertexData& C);
	bool isOnScreen();
	void suthHodgeCrop();
	void createTriangleSides(VertexData A, VertexData B, VertexData C);
	void drawHalfTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps, Screen* screenptr);
};

