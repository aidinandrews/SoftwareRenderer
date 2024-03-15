#pragma once
#include "vec3.h"
#include "vec2.h"
#include "screen.h"
#include "camera.h"
#include "swap.h"
#include "color.h"
#include "line.h"
#include "SDL.h"
#include "bmp.h"
#include "EasyBMP.h"

class Triangle {

public:
	point3 _mapPoints[3];
	Uint32BMP* p_texture = nullptr;
	point2 _texturePos[3];
	Uint32 _color;
	vec3 _normal[3];
	bool _backFaceCulling;

public:
	Triangle();
	Triangle(point3 A, point3 B, point3 C, Uint32 color, Screen& screen, Camera& camera);
	~Triangle();
	
	void setPos(point3 p1, point3 p2, point3 p3);
	void move(float x, float y, float z);
	void scale(float value, point3 center);
	void rotate(float rotateX, float rotateY, float rotateZ, point3 center);
	void setColor(Uint32 color);
	point3 centroid();
	void setNormal();
};

