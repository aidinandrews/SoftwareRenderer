#pragma once
#include "plane.h"
#include "vec3.h"
#include <math.h>

class Plane {

public:
	float A, B, C, D;

public:
	Plane();
	Plane(vec3 normal, point3 p);
	~Plane();

	void setPlane(point3 p1, point3 p2, point3 p3);
	void setPlane(vec3 normal, point3 p1);
};

float pointToPlaneDist(point3 point, Plane p);