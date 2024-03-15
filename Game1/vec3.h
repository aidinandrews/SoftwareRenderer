#pragma once
#include <cmath>
#include <iostream>

class vec3 {

public:
	float e[3];

public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

	float& x() { return e[0]; }
	float& y() { return e[1]; }
	float& z() { return e[2]; }

	void rotate(float rotateX, float rotateY, float rotateZ, vec3 center);
	void scale(float amount, vec3 center);
	void move(vec3 distance);

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	vec3& operator=(const float v) {
		e[0] = v;
		e[1] = v;
		e[2] = v;
		return *this;
	}

	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator-=(const vec3& v) {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}

	vec3& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const float t) {
		return *this *= 1 / t;
	}

	float lengthSquared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	float length() const {
		return sqrt(lengthSquared());
	}
};

// Other names for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << "(" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ") ";
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const vec3& v, const float t) {
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(const float t, const vec3& v) {
	return v * t;
}

inline vec3 operator/(vec3 v, float t) {
	return v * (1 / t);
}

inline bool operator!=(vec3 v, vec3 t) {
	return v.x() == t.x() && v.y() == t.y() && v.z() == t.z();
}

inline bool operator!=(vec3 v, int t) {
	return v.x() == t && v.y() == t && v.z() == t;
}

inline float dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unitVector(vec3 v) {
	return v / v.length();
}

inline vec3 getNormal(point3 p1, point3 p2, point3 p3) {
	return unitVector(cross(p2 - p1, p3 - p1));
}

point3 rotateAroundCenter(point3 point, float rotateX, float rotateY, float rotateZ);