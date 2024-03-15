#pragma once

inline void swap(int& a, int& b) {
	int temp = a; a = b; b = temp;
}

inline void swap(float& a, float& b) {
	float temp = a; a = b; b = temp;
}

inline void swap(point2& a, point2& b) {
	point2 temp = a; a = b; b = temp;
}

inline void swap(point3& a, point3& b) {
	point3 temp = a; a = b; b = temp;
}
