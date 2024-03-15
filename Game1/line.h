#pragma once
#include <iostream>
#include <cmath>
#include "Screen.h"
#include "vertexData.h"

void drawLine(Screen& screen, point2 A, point2 B, Uint32 color);
void drawAALine(Screen& screen, point2 A, point2 B, float value);

float getLineDist2d(point2 A, point2 B);

float getLineDist3d(point3 A, point3 B);

struct LineSegment {
	int steps = 0;
	VertexData current, offset;

	void offsetData();
	void createSide(VertexData A, VertexData B);
	void createScanline(VertexData leftPoint, VertexData rightPoint);
};
