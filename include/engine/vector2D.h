#pragma once

#include <cmath>
#include <ostream>

struct Vec2 {
	float x;
	float y;

	Vec2(const float& x, const float& y);
	Vec2(const int& x, const int& y);
	Vec2();
	Vec2(const float& degrees);
	Vec2(const Vec2& vec);

	float magnitude() const;
	Vec2 normalized() const;
	Vec2 clamped(const float& maxMagnitude) const;

	float dotProduct(const Vec2& other) const;

	int toDegrees() const;

	Vec2 rotateAround(Vec2 point, float degrees) const;

	bool operator==(const Vec2& rhs) const;

	bool operator<(const Vec2& rhs) const;
	bool operator>(const Vec2& rhs) const;
};
//
// Operator overloading
// + and +=
inline Vec2& operator+=(Vec2& lhs, const Vec2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Vec2 operator+(Vec2 lhs, const Vec2& rhs) {
	lhs += rhs;
	return lhs;
}

// - and -= overloading
inline Vec2& operator-=(Vec2& lhs, const Vec2& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline Vec2 operator-(Vec2 lhs, const Vec2& rhs) {
	lhs -= rhs;
	return lhs;
}

// * and *= overloading
inline Vec2& operator*=(Vec2& lhs, const Vec2& rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	return lhs;
}

inline Vec2& operator*=(Vec2& lhs, const double& rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

inline Vec2 operator*(Vec2 lhs, const Vec2& rhs) {
	lhs *= rhs;
	return lhs;
}

inline Vec2 operator*(Vec2 lhs, const double& rhs) {
	lhs *= rhs;
	return lhs;
}

// << overloading
inline std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}
