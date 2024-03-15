#include "trianglePointData.h"

void swap(TrianglePointData* A, TrianglePointData* B) {
	TrianglePointData* temp = A; A = B; B = temp;
}
void swap(TrianglePointData& A, TrianglePointData& B) {
	TrianglePointData temp = A; A = B; B = temp;
}

TrianglePointData addShadows(TrianglePointData A, TrianglePointData B) {
	TrianglePointData answer;
	answer.shadow = A.shadow + B.shadow;
	answer.lightDist = A.lightDist + B.lightDist;
	return answer;
}
TrianglePointData subtractShadows(TrianglePointData A, TrianglePointData B) {
	TrianglePointData answer;
	answer.shadow = A.shadow - B.shadow;
	answer.lightDist = A.lightDist - B.lightDist;
	return answer;
}
TrianglePointData multiplyShadows(TrianglePointData A, float v) {
	TrianglePointData answer;
	answer.shadow = A.shadow * v;
	answer.lightDist = A.lightDist * v;
	return answer;
}
TrianglePointData divideShadows(TrianglePointData A, float v) {
	return multiplyShadows(A, 1/v);
}