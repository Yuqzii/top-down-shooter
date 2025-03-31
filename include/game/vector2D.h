#pragma once

#include <cmath>

struct vector2Df {
	float x;
	float y;

	vector2Df(const float& x, const float& y);
	vector2Df();
	vector2Df(const float& degrees);
	vector2Df(const vector2Df& vec);

	float magnitude() const;
	vector2Df normalized() const;
	vector2Df clamped(const float& maxMagnitude) const;

	int toDegrees() const;

	vector2Df rotateAround(vector2Df point, float degrees) const;
};

struct vector2D {
	int x;
	int y;

	vector2D(int x, int y);
	vector2D();

	vector2D& operator=(const vector2Df& other);

	vector2D& operator=(const vector2D& other);
};

// Operator overloading
// + and +=
inline vector2Df& operator+=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline vector2Df& operator+=(vector2Df& lhs, const vector2D& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline vector2D& operator+=(vector2D& lhs, const vector2D& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline vector2D& operator+=(vector2D& lhs, const vector2Df& rhs) {
	lhs.x += round(rhs.x);
	lhs.y += round(rhs.y);
	return lhs;
}

inline vector2Df operator+(vector2Df lhs, const vector2Df& rhs) {
	lhs += rhs;
	return lhs;
}

inline vector2Df operator+(vector2Df lhs, const vector2D& rhs) {
	lhs += rhs;
	return lhs;
}

inline vector2D operator+(vector2D lhs, const vector2D& rhs) {
	lhs += rhs;
	return lhs;
}

inline vector2D operator+(vector2D lhs, const vector2Df& rhs) {
	lhs += rhs;
	return lhs;
}

// - and -= overloading
inline vector2Df& operator-=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline vector2Df& operator-=(vector2Df& lhs, const vector2D& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline vector2D& operator-=(vector2D& lhs, const vector2Df& rhs) {
	lhs.x = round(lhs.x - rhs.x);
	lhs.y = round(lhs.y - rhs.y);
	return lhs;
}

inline vector2D& operator-=(vector2D& lhs, const vector2D& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline vector2Df operator-(vector2Df lhs, const vector2Df& rhs) {
	lhs -= rhs;
	return lhs;
}

inline vector2Df operator-(vector2Df lhs, const vector2D& rhs) {
	lhs -= rhs;
	return lhs;
}

inline vector2D operator-(vector2D lhs, const vector2Df& rhs) {
	lhs -= rhs;
	return lhs;
}

inline vector2D operator-(vector2D lhs, const vector2D& rhs) {
	lhs -= rhs;
	return lhs;
}

// * and *= overloading
inline vector2Df& operator*=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline vector2Df& operator*=(vector2Df& lhs, const vector2D& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline vector2Df& operator*=(vector2Df& lhs, const double& rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline vector2D& operator*=(vector2D& lhs, const int& rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline vector2D& operator*=(vector2D& lhs, const vector2Df& rhs) {
	lhs.x = round(lhs.x * rhs.x);
	lhs.y = round(lhs.y * rhs.y);
	return lhs;
}

inline vector2D& operator*=(vector2D& lhs, const vector2D& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline vector2Df operator*(vector2Df lhs, const vector2Df& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2Df operator*(vector2Df lhs, const vector2D& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2Df operator*(vector2Df lhs, const double& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2D operator*(vector2D lhs, const vector2Df& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2D operator*(vector2D lhs, const vector2D& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2D operator*(vector2D lhs, const int& rhs) {
	lhs *= rhs;
	return lhs;
}
