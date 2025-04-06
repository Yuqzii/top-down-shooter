#pragma once

#include <SDL2/SDL_rect.h>
#include "game/vector2D.h"

class SDL_Renderer;

namespace Collision {
struct Circle {
	vector2D position;
	float radius;

	Circle() : position(), radius(0.0f) {}
	Circle(const vector2D& pos, const float r) : position(pos), radius(r) {}
};
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
bool checkCollision(const Circle& a, const Circle& b);

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider);
}
