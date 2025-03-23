#include "game/collision.h"

namespace Collision {

namespace {
int distanceSquared(const vector2D& a, const vector2D& b) {
	int deltaX = b.x - a.x;
	int deltaY = b.y - a.y;
	return deltaX * deltaX + deltaY * deltaY;
}
}

// Returns true if rect a overlaps with rect b
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.y + a.h <= b.y || a.y >= b.y + b.h || a.x + a.w <= b.x || a.x >= b.x + b.w) {
		return false;
	}
	else {
		return true;
	}
}

bool checkCollision(const Circle& a, const Circle& b) {
	// We can use distance squared instead of just distance to optimize,
	// because getting the normal distance requires square root (an expensive operation),
	// but if x > y then x^2 > y^2, so we can square both values and compare instead
	int totalRadiusSquared = (a.radius + b.radius);
	totalRadiusSquared *= totalRadiusSquared;

	// If distance between center of circles is less than sum of their radii
	if (distanceSquared(a.position, b.position) < totalRadiusSquared) {
		return true;
	}
	return false;
}

}
