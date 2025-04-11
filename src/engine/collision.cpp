#include "engine/collision.h"
#include "SDL2/SDL_render.h"

namespace Collision {

namespace {
int distanceSquared(const vector2D& a, const vector2D& b) {
	int deltaX = b.x - a.x;
	int deltaY = b.y - a.y;
	return deltaX * deltaX + deltaY * deltaY;
}

int roundUpToMultipleOfEight(int x) {
	return (x + (8 - 1)) & -8;
}

vector2Df closestPointOnLine(const vector2Df& point, const Line& line) {
	vector2Df tangent = line.end - line.start;

	if ((point - line.start).crossProduct(tangent) <= 0) {
		return line.start; // point is at or before start of line
	}

	if ((point - line.end).crossProduct(tangent) >= 0) {
		return line.end; // point is at after end of line
	}

	tangent = tangent.normalized();
	const vector2Df relativePos = point - line.start;
	return line.start + tangent * (tangent.crossProduct(relativePos));
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

bool checkCollision(const vector2Df& point, const Circle& c) {
	return distanceSquared(point, c.position) <= c.radius * c.radius;
}

bool checkCollision(const Circle& c, const Line& l) {
	// Are any of the endpoints inside the circle?
	if (checkCollision(l.start, c) || checkCollision(l.end, c)) return true;

	const vector2Df delta = c.position - closestPointOnLine(c.position, l);

	// Is distance from closest point to circle larger than its radius?
	return delta.crossProduct(delta) <= c.radius * c.radius;
}

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider) {
	const int arrSize = roundUpToMultipleOfEight(collider.radius * 8 * 35 / 49);
	SDL_Point points[arrSize];
	int drawCount = 0;

	const int32_t diameter = (collider.radius * 2);

	int32_t x = collider.radius - 1;
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = tx - diameter;

	while (x >= y) {
		// Each of the following renders and octant of the circle
		points[drawCount+0] = { collider.position.x + x, collider.position.y - y };
        points[drawCount+1] = { collider.position.x + x, collider.position.y + y };
        points[drawCount+2] = { collider.position.x - x, collider.position.y - y };
        points[drawCount+3] = { collider.position.x - x, collider.position.y + y };
        points[drawCount+4] = { collider.position.x + y, collider.position.y - x };
        points[drawCount+5] = { collider.position.x + y, collider.position.y + x };
        points[drawCount+6] = { collider.position.x - y, collider.position.y - x };
        points[drawCount+7] = { collider.position.x - y, collider.position.y + x };

        drawCount += 8;

		if (error <= 0) {
			y++;
			error += ty;
			ty += 2;
		}
		
		if (error > 0) {
			x--;
			tx += 2;
			error += tx - diameter;
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawPoints(renderer, points, drawCount);
}

}
