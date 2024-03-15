#pragma once
#include "vec3.h"
#include "vec2.h"

struct TrianglePointData {
	point3 map;
	point2 screen;	
	point2 texture;
	float camDist = 0;
	point2 shadow;
	float lightDist = 0;
	vec3 normal;
};

void swap(TrianglePointData* A, TrianglePointData* B);
void swap(TrianglePointData& A, TrianglePointData& B);

inline TrianglePointData& operator+(const TrianglePointData A, const TrianglePointData B) {
	TrianglePointData answer;
		answer.map = A.map + B.map;
		answer.screen = A.screen + B.screen;
		answer.texture = A.texture + B.texture;
		answer.camDist = A.camDist + B.camDist;
		answer.shadow = A.shadow + B.shadow;
		answer.lightDist = A.lightDist + B.lightDist;
		answer.normal = A.normal + B.normal;
	return answer;
}
inline TrianglePointData& operator-(const TrianglePointData A, const TrianglePointData B) {
	TrianglePointData answer;
	answer.map = A.map - B.map;
	answer.screen = A.screen - B.screen;
	answer.texture = A.texture - B.texture;
	answer.camDist = A.camDist - B.camDist;
	answer.shadow = A.shadow - B.shadow;
	answer.lightDist = A.lightDist - B.lightDist;
	answer.normal = A.normal - B.normal;
	return answer;
}
inline TrianglePointData operator*(const TrianglePointData A, const float v) {
	TrianglePointData answer;
	answer.map = A.map * v;
	answer.screen = A.screen * v;
	answer.texture = A.texture * v;
	answer.camDist = A.camDist * v;
	answer.shadow = A.shadow * v;
	answer.lightDist = A.lightDist * v;
	answer.normal = A.normal * v;
	return answer;
}
inline TrianglePointData operator*(const float v, const TrianglePointData A) {
	return A * v;
}
inline TrianglePointData operator/(const TrianglePointData A, const float v) {
	return A * (1 / v);
}

TrianglePointData addShadows(TrianglePointData A, TrianglePointData B);
TrianglePointData subtractShadows(TrianglePointData A, TrianglePointData B);
TrianglePointData multiplyShadows(TrianglePointData A, float v);
TrianglePointData divideShadows(TrianglePointData A, float v);