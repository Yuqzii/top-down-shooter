#pragma once

#include <cmath>
#include <ostream>

struct vector2Df {
	float x;
	float y;

	vector2Df(const float& x, const float& y);
	vector2Df(const int& x, const int& y);
	vector2Df();
	vector2Df(const float& degrees);
	vector2Df(const vector2Df& vec);

	float magnitude() const;
	vector2Df normalized() const;
	vector2Df clamped(const float& maxMagnitude) const;

	float dotProduct(const vector2Df& other) const;

	int toDegrees() const;

	vector2Df rotateAround(vector2Df point, float degrees) const;

	bool operator==(const vector2Df& rhs) const;

	bool operator<(const vector2Df& rhs) const;
	bool operator>(const vector2Df& rhs) const;
};
//
// Operator overloading
// + and +=
inline vector2Df& operator+=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline vector2Df operator+(vector2Df lhs, const vector2Df& rhs) {
	lhs += rhs;
	return lhs;
}

// - and -= overloading
inline vector2Df& operator-=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline vector2Df operator-(vector2Df lhs, const vector2Df& rhs) {
	lhs -= rhs;
	return lhs;
}

// * and *= overloading
inline vector2Df& operator*=(vector2Df& lhs, const vector2Df& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline vector2Df& operator*=(vector2Df& lhs, const double& rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline vector2Df operator*(vector2Df lhs, const vector2Df& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2Df operator*(vector2Df lhs, const double& rhs) {
	lhs *= rhs;
	return lhs;
}

// << overloading
inline std::ostream& operator<<(std::ostream& os, const vector2Df& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}
