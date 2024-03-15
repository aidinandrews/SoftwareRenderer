#pragma once
#include <cmath>
#include <iostream>
#include "vec3.h"

class vec2 {

public:
	float e[2];

public:
	vec2() : e{ 0, 0 } {}
	vec2(float e0, float e1) : e{ e0, e1 } {}

	float& x() { return e[0]; }
	float& y() { return e[1]; }

	void rotate(float radians, vec2 center);

	vec2 operator-() const { return vec2(-e[0], -e[1]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	vec2 operator=(const vec3& v) {
		e[0] = v.e[0];
		e[1] = v.e[1];
		return *this;
	}

	vec2& operator+=(const vec2& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		return *this;
	}

	vec2& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		return *this;
	}

	vec2& operator-=(const vec2& v) {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		return *this;
	}
	vec2& operator-=(const vec3& v) {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		return *this;
	}

	vec2& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		return *this;
	}

	vec2& operator/=(const float t) {
		return *this *= 1 / t;
	}

	float lengthSquared() const {
		return e[0] * e[0] + e[1] * e[1];
	}

	float length() const {
		return sqrt(lengthSquared());
	}
};

// Other names for vec2
using point2 = vec2; // 2D point

// vec2 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec2& v) {
	return out << "(" << v.e[0] << ", " << v.e[1] << ") ";
}

inline vec2 operator+(const vec2& u, const vec2& v) {
	return vec2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}

inline vec2 operator-(const vec2& u, const vec2& v) {
	return vec2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}

inline vec2 operator*(const vec2& u, const vec2& v) {
	return vec2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}

inline vec2 operator*(const vec2& v, const float t) {
	return vec2(v.e[0] * t, v.e[1] * t);
}

inline vec2 operator*(const float t, const vec2& v) {
	return v * t;
}

inline vec2 operator/(vec2 v, float t) {
	return v * (1 / t);
}

inline float dot(const vec2& u, const vec2& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1];
}

inline vec2 unitVector(vec2 v) {
	return v / v.length();
}