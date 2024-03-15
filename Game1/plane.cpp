#include "plane.h"

Plane::Plane() : A(0), B(0), C(0), D(0) {}

Plane::Plane(vec3 normal, point3 p) {
	setPlane(normal, p);
}

Plane::~Plane() {}

void Plane::setPlane(point3 p1, point3 p2, point3 p3) {

	float a1 = p2.x() - p1.x();
	float b1 = p2.y() - p1.y();
	float c1 = p2.z() - p1.z();
	float a2 = p3.x() - p1.x();
	float b2 = p3.y() - p1.y();
	float c2 = p3.z() - p1.z();
	A = b1 * c2 - b2 * c1;
	B = a2 * c1 - a1 * c2;
	C = a1 * b2 - b1 * a2;
	D = (-A * p1.x() - B * p1.y() - C * p1.z());
}

void Plane::setPlane(vec3 normal, point3 p1) {
	//Ax + By + Cz + D = 0
	A = normal.x();
	B = normal.y();
	C = normal.z();
	D = -1 * (normal.x() * p1.x() + normal.y() * p1.y() + normal.z() * p1.z());
}

float pointToPlaneDist(point3 point, Plane p) {

	p.D = fabs(p.A * point.x() + p.B * point.y() + p.C * point.z() + p.D);
	float e = sqrt(p.A * p.A + p.B * p.B + p.C * p.C);
	float dist = p.D / e;
	return dist;
}
