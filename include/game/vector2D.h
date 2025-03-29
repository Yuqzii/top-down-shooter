#pragma once

#include <cmath>

struct vector2Df {
	float x;
	float y;

	vector2Df(const float& x, const float& y);
	vector2Df();
	vector2Df(const float& degrees);
	vector2Df(const vector2Df& vec);

	void normalize();

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
