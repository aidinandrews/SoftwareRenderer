#include "triangle.h"

Triangle::Triangle() : _color(0xFF00FFFF), _backFaceCulling(true) {}

Triangle::Triangle(point3 A, point3 B, point3 C, Uint32 color, Screen& screen, Camera& camera) 
	: _color(color), _backFaceCulling(true) {

	_mapPoints[0] = A;
	_mapPoints[1] = B;
	_mapPoints[2] = C;
	_normal[0] = getNormal(_mapPoints[0], _mapPoints[1], _mapPoints[2]);
	_normal[0] = unitVector(_normal[0]);
	_normal[1] = _normal[0];
	_normal[2] = _normal[0];
}

Triangle::~Triangle() {}

void Triangle::setNormal() {
	_normal[0] = getNormal(_mapPoints[0], _mapPoints[1], _mapPoints[2]);
	_normal[0] = unitVector(_normal[0]);
	_normal[1] = _normal[0];
	_normal[2] = _normal[0];
}

point3 Triangle::centroid() {
	point3 centroid = (_mapPoints[0] + _mapPoints[1] + _mapPoints[0]) / 3;
	return centroid;
}
void Triangle::setPos(point3 p1, point3 p2, point3 p3) {
	_mapPoints[0] = p1; _mapPoints[1] = p2; _mapPoints[2] = p3;
	setNormal();
}
void Triangle::setColor(Uint32 color) {
	_color = color;
}
void Triangle::move(float x, float y, float z) {
	vec3 v(x, y, z);
	_mapPoints[0] += v;
	_mapPoints[1] += v;
	_mapPoints[2] += v;
}
void Triangle::scale(float value, point3 center) {
	_mapPoints[0] = (_mapPoints[0] - center) * value + center;
	_mapPoints[1] = (_mapPoints[1] - center) * value + center;
	_mapPoints[2] = (_mapPoints[2] - center) * value + center;
}
void Triangle::rotate(float rotateX, float rotateY, float rotateZ, point3 center) {
	_mapPoints[0].rotate(rotateX, rotateY, rotateZ, center);
	_mapPoints[1].rotate(rotateX, rotateY, rotateZ, center);
	_mapPoints[2].rotate(rotateX, rotateY, rotateZ, center);
	setNormal();
}

