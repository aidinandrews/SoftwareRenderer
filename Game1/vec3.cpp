#include "vec3.h"

void vec3::rotate(float rotateX, float rotateY, float rotateZ, vec3 center) {

	*this -= center;

	float newX, newY, newZ;
	//rotate around z axis
	newX = (e[0] * cos(rotateZ) - e[1] * sin(rotateZ));
	newY = (e[1] * cos(rotateZ) + e[0] * sin(rotateZ));
	e[0] = newX;
	e[1] = newY;
	//rotate around y axis
	newX = (e[0] * cos(rotateY) - e[2] * sin(rotateY));
	newZ = (e[2] * cos(rotateY) + e[0] * sin(rotateY));
	e[0] = newX;
	e[2] = newZ;
	//rotate around x axis
	newY = (e[1] * cos(rotateX) - e[2] * sin(rotateX));
	newZ = (e[2] * cos(rotateX) + e[1] * sin(rotateX));
	e[1] = newY;
	e[2] = newZ;

	*this += center;
}

point3 rotateAroundCenter(point3 point, float rotateX, float rotateY, float rotateZ) {

	float newX, newY, newZ;
	//rotate around z axis
	newX = (point.x() * cos(rotateZ) - point.y() * sin(rotateZ));
	newY = (point.y() * cos(rotateZ) + point.x() * sin(rotateZ));
	point.x() = newX;
	point.y() = newY;
	//rotate around y axis
	newX = (point.x() * cos(rotateY) - point.z() * sin(rotateY));
	newZ = (point.z() * cos(rotateY) + point.x() * sin(rotateY));
	point.x() = newX;
	point.z() = newZ;
	//rotate around x axis
	newY = (point.y() * cos(rotateX) - point.z() * sin(rotateX));
	newZ = (point.z() * cos(rotateX) + point.y() * sin(rotateX));
	point.y() = newY;
	point.z() = newZ;

	return point;
}

void vec3::scale(float amount, vec3 center) {
	*this = (*this - center) * amount + center;
}

void vec3::move(vec3 distance){
	e[0] += distance[0];
	e[1] += distance[1];
	e[2] += distance[2];
}
