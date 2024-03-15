#pragma once
#include "vec3.h"
#include "vec2.h"
#include "camera.h"
#include "screen.h"
#include "triangle.h"
#include "color.h"
#include <iomanip>
#include <cmath>
#include "bmp.h"
#include "trianglePointData.h"

class Renderer {
public:
	Uint32BMP* m_defaultTexture;
	bool awayFacing = false;

	struct LineSegment {
		int steps = 0;
		TrianglePointData current, offset;

		void offsetData();
		void createSide(TrianglePointData A, TrianglePointData B);
		void createScanline(TrianglePointData leftPoint, TrianglePointData rightPoint);

		void offsetShadowData();
		void createShadowSide(TrianglePointData A, TrianglePointData B);
		void createShadowScanline(TrianglePointData leftPoint, TrianglePointData rightPoint);
	};

private:
	Screen* p_screen;
	Camera* p_camera;
	Uint32BMP* p_texture;
	Triangle* p_triangle;
	const Uint8* state = SDL_GetKeyboardState(NULL);
	TrianglePointData m_points[7];
	LineSegment m_leftSide, m_rightSide;
	Uint32 m_color;
public:
	Renderer();
	Renderer(Screen* screen, Camera* camera) : p_screen(screen), p_camera(camera) {

	}
	~Renderer();
	void init(Screen* screen, Camera* camera);
	Screen* getScreen() {
		return p_screen;
	}
	Camera* getCamera() {
		return p_camera;
	}

	point2 project(const point3& mapPosition);

	void renderTriangle(Triangle* triangle);
	void renderTriangle(TrianglePointData& A, TrianglePointData& B, TrianglePointData& C, bool backFaceCulling);
	void drawTriangle(TrianglePointData A, TrianglePointData B, TrianglePointData C);
	void drawHalfTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps);

	void renderLine(point2 A, point2 B, int width);
	void renderLine(point3 A, point3 B, int width);

	void renderShadowMap(TrianglePointData A, TrianglePointData B, TrianglePointData C);
	void drawShadowTriangle(TrianglePointData A, TrianglePointData B, TrianglePointData C);
	void drawHalfShadowTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps);

	void suthHodgeCrop();
	void antiAlias();
};


