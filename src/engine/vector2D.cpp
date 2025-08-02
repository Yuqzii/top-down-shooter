#include "engine/vector2D.h"

#include <cmath>

// vector2Df
Vec2::Vec2(const float x, const float y) {
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const int x, const int y) {
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const std::size_t x, const std::size_t y) {
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
}

Vec2::Vec2() { x = y = 0; }

Vec2::Vec2(const float degrees) {  // Creates direction vector from rotation in degrees
	float radians = (degrees - 90) * M_PI / 180;
	x = std::cos(radians);
	y = std::sin(radians);
}

Vec2::Vec2(const Vec2& vec) {
	x = vec.x;
	y = vec.y;
}

float Vec2::magnitude() const { return std::sqrt(x * x + y * y); }

Vec2 Vec2::normalized() const {
	float mag = magnitude();
	Vec2 res(*this);
	if (mag > 0) {
		res.x /= mag;
		res.y /= mag;
	}
	return res;
}

Vec2 Vec2::clamped(const float& maxMagnitude) const {
	float mag = magnitude();
	Vec2 res(*this);
	if (mag > maxMagnitude) {
		res.x /= mag;
		res.y /= mag;

		res *= maxMagnitude;
	}
	return res;
}

int Vec2::toDegrees() const { return std::atan2(y, x) * 180 / M_PI; }

float Vec2::dotProduct(const Vec2& other) const { return x * other.x + y * other.y; }

Vec2 Vec2::rotateAround(Vec2 point, float degrees) const {
	float radians = (degrees)*M_PI / 180;  // Convert angle to radians

	// Get sine and cosine values
	float s = std::sin(radians);
	float c = std::cos(radians);

	// Translate point back to origin
	Vec2 rotated(*this);
	rotated -= point;

	// Rotate point
	Vec2 newRot(rotated.x * c - rotated.y * s, rotated.x * s + rotated.y * c);

	// Move point back
	rotated = newRot + point;

	return rotated;
}

bool Vec2::operator==(const Vec2& rhs) const { return this->x == rhs.x && this->y == rhs.y; }

bool Vec2::operator<(const Vec2& rhs) const { return this->x < rhs.x; }

bool Vec2::operator>(const Vec2& rhs) const { return this->x > rhs.x; }
