#include "game/vector2D.h"

// + and += overloading
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

inline vector2D operator*(vector2D lhs, const vector2Df& rhs) {
	lhs *= rhs;
	return lhs;
}

inline vector2D operator*(vector2D lhs, const vector2D& rhs) {
	lhs *= rhs;
	return lhs;
}

// vector2Df
vector2Df::vector2Df(const float& x, const float& y) {
	this->x = x;
	this->y = y;
}
vector2Df::vector2Df() {
	x = y = 0;
}
vector2Df::vector2Df(const float& degrees) { // Creates direction vector from rotation in degrees
	float radians = (degrees - 90) * M_PI / 180;
	x = std::cos(radians);
	y = std::sin(radians);
}
vector2Df::vector2Df(const vector2Df& vec) {
	x = vec.x;
	y = vec.y;
}

void vector2Df::normalize() {
	float length = std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0));
	if (length > 0) {
		x /= length;
		y /= length;
	}
}

int vector2Df::toDegrees() const {
	return std::atan2(y, x) * 180 / M_PI;
}

vector2Df vector2Df::rotateAround(vector2Df point, float degrees) const {
	float radians = (degrees) * M_PI / 180; // Convert angle to radians

	// Get sine and cosine values
	float s = std::sin(radians);
	float c = std::cos(radians);

	// Translate point back to origin
	vector2Df rotated(*this);
	rotated -= point;

	// Rotate point
	vector2Df newRot(rotated.x * c - rotated.y * s, rotated.x * s + rotated.y * c);

	// Move point back
	rotated = newRot + point;

	return rotated;
}

// vector2D
vector2D::vector2D(int x, int y) {
	this->x = x;
	this->y = y;
}
vector2D::vector2D() {
	x = y = 0;
}

vector2D& vector2D::operator=(const vector2Df& other) {
	x = round(other.x);
	y = round(other.y);
	return *this;
}

vector2D& vector2D::operator=(const vector2D& other) {
	if (this != &other) {
		x = other.x;
		y = other.y;
	}
	return *this;
}

