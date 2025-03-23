#pragma once

#include <SDL2/SDL_rect.h>
#include "game/vector2D.h"

namespace Collision {
struct Circle {
	vector2D position;
	float radius;
};
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
bool checkCollision(const Circle& a, const Circle& b);
}
