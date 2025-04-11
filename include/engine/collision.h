#pragma once

#include <SDL2/SDL_rect.h>
#include "engine/vector2D.h"

class SDL_Renderer;

namespace Collision {

enum class Types {
	CIRCLE = 0,
	// POINT is only used checking collision that is not part of standard collider,
	// not in a "full" GameObject.
	POINT,
};

struct Circle {
	vector2D position;
	float radius;

	Circle() : position(), radius(0.0f) {}
	Circle(const vector2D& pos, const float r) : position(pos), radius(r) {}
};

struct Line {
	const vector2Df start;
	const vector2Df end;

	Line() : start(), end() {}
	Line(const vector2Df& s, const vector2Df& e) : start(s), end(e) {}
};

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
bool checkCollision(const Circle& a, const Circle& b);
bool checkCollision(const vector2Df& point, const Circle& circle);
bool checkCollision(const Circle& circle, const Line& line);

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider);
}
