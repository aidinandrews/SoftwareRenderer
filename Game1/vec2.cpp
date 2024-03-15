#include "vec2.h"

void vec2::rotate(float radians, vec2 center) {
	*this -= center;

	float newX, newY;
	//rotate around z axis
	newX = (e[0] * cos(radians) - e[1] * sin(radians));
	newY = (e[1] * cos(radians) + e[0] * sin(radians));
	e[0] = newX;
	e[1] = newY;

	*this += center;
}
