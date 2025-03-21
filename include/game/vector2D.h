#pragma once

#include <cmath>

struct vector2D {
	int x;
	int y;

	vector2D(int x, int y) {
		this->x = x;
		this->y = y;
	}
	vector2D() {
		x = y = 0;
	}
};

struct vector2Df {
	float x;
	float y;

	vector2Df(float x, float y) {
		this->x = x;
		this->y = y;
	}
	vector2Df() {
		x = y = 0;
	}

	void normalize() {
		float length = std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0));
		if (length > 0) {
			x /= length;
			y /= length;
		}
	}
};
