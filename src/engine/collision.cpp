#include "engine/collision.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "SDL2/SDL_render.h"
#include "engine/scene.h"
#include "engine/gameObject.h"

namespace Collision {

namespace {
int distanceSquared(const vector2D& a, const vector2D& b) {
	int deltaX = b.x - a.x;
	int deltaY = b.y - a.y;
	return deltaX * deltaX + deltaY * deltaY;
}

int roundUpToMultipleOfEight(int x) { return (x + (8 - 1)) & -8; }

vector2Df closestPointOnLine(const vector2Df& point, const Line& line) {
	vector2Df tangent = line.end - line.start;

	if ((point - line.start).crossProduct(tangent) <= 0) {
		return line.start;	// point is at or before start of line
	}

	if ((point - line.end).crossProduct(tangent) >= 0) {
		return line.end;  // point is at after end of line
	}

	tangent = tangent.normalized();
	const vector2Df relativePos = point - line.start;
	return line.start + tangent * (tangent.crossProduct(relativePos));
}

// Given three collinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool onSegment(const vector2Df& p, const vector2Df& q, const vector2Df& r) {
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
		return true; 
  
    return false;
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
int orientation(const vector2Df& p, const vector2Df& q, const vector2Df& r) {
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    const int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // collinear 
  
    return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

}  // namespace

// Returns true if rect a overlaps with rect b
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.y + a.h <= b.y || a.y >= b.y + b.h || a.x + a.w <= b.x || a.x >= b.x + b.w) {
		return false;
	} else {
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

// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool checkCollision(const Line& a, const Line& b) {
	const int o1 = orientation(a.start, a.end, b.start);
	const int o2 = orientation(a.start, a.end, b.end);
	const int o3 = orientation(b.start, b.end, a.start);
	const int o4 = orientation(b.start, b.end, a.end);

	if (o1 != o2 && o3 != o4)
		return true;

    // Special Cases 
	// a.start, a.end and b.start are collinear and b.start lies on segment a
	if (o1 == 0 && onSegment(a.start, b.start, a.end)) return true; 

	// a.start, a.end and b.end are collinear and b.end lies on segment a
	if (o2 == 0 && onSegment(a.start, b.end, a.end)) return true; 

	// b.start, b.end and a.start are collinear and a.start lies on segment b
	if (o3 == 0 && onSegment(b.start, a.start, b.end)) return true; 

	// b.start, b.end and a.end are collinear and a.end lies on segment b
	if (o4 == 0 && onSegment(b.start, a.end, b.end)) return true; 

	return false; // Doesn't fall in any of the above cases 
}

bool checkCollision(const vector2Df& a, const vector2Df& b) {
	return a == b;
}

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider) {
	const int arrSize = roundUpToMultipleOfEight(collider.radius * 8 * 35 / 49);
	std::vector<SDL_Point> points(arrSize);
	int drawCount = 0;

	const int32_t diameter = (collider.radius * 2);

	int32_t x = collider.radius - 1;
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = tx - diameter;

	while (x >= y) {
		// Each of the following renders and octant of the circle
		points[drawCount + 0] = {(int)collider.position.x + x, (int)collider.position.y - y};
		points[drawCount + 1] = {(int)collider.position.x + x, (int)collider.position.y + y};
		points[drawCount + 2] = {(int)collider.position.x - x, (int)collider.position.y - y};
		points[drawCount + 3] = {(int)collider.position.x - x, (int)collider.position.y + y};
		points[drawCount + 4] = {(int)collider.position.x + y, (int)collider.position.y - x};
		points[drawCount + 5] = {(int)collider.position.x + y, (int)collider.position.y + x};
		points[drawCount + 6] = {(int)collider.position.x - y, (int)collider.position.y - x};
		points[drawCount + 7] = {(int)collider.position.x - y, (int)collider.position.y + x};

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
	SDL_RenderDrawPoints(renderer, &points[0], drawCount);
}

}

Collider::Collider(const Collision::Types collisionType_, const float checkRadius_,
				   GameObject* parent_)
	: collisionType{collisionType_}, checkRadius{checkRadius_}, parent{parent_} {}

Collider::Collider(const Collision::Types collisionType_, const float checkRadius_)
	: Collider{collisionType_, checkRadius_, nullptr} {}

CircleCollider::CircleCollider(Collision::Circle circle_, const float checkRadius_,
							   GameObject* parent)
	: circle{std::move(circle_)}, Collider{Collision::Types::CIRCLE, checkRadius_, parent} {}

CircleCollider::CircleCollider(Collision::Circle circle_, const float checkRadius_)
	: CircleCollider{std::move(circle_), checkRadius_, nullptr} {}

LineCollider::LineCollider(Collision::Line line_, const float checkRadius_, GameObject* parent)
	: line{std::move(line_)}, Collider{Collision::Types::LINE, checkRadius_, parent} {}

LineCollider::LineCollider(Collision::Line line_, const float checkRadius_)
	: LineCollider{std::move(line_), checkRadius_, nullptr} {}

PointCollider::PointCollider(vector2Df point_, const float checkRadius_, GameObject* parent)
	: point{std::move(point_)}, Collider{Collision::Types::POINT, checkRadius_, parent} {}

PointCollider::PointCollider(vector2Df point_, const float checkRadius_)
	: PointCollider{std::move(point_), checkRadius_, nullptr} {}

void Collider::collisionUpdate() {
	for (const Collider* other : collisionList) {
		if (other == nullptr) // Ensure that other collider exists
			continue;

		try {
			onCollision(*other);
		} catch (int e) {
			// Stop collision detection when throwing exception
			break;
		}
	}

	collisionList.clear(); // Make sure collisionList only contains collisions from one frame
}

void Collider::addCollision(const Collider* other) {
	collisionList.insert(other);
}

void Collider::onCollision(const Collider& other) {
	if (parent == nullptr) return;
	parent->onCollision(other);
}

void CircleCollider::checkCollisions(const Scene& scene) {
	std::vector<GameObject*> closeObjects;
	try {
		// Get all GameObjects withing our bounding circle
		closeObjects = scene.getObjectTree().findObjectsInRange(circle.position, checkRadius);
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || collisionList.count(otherCollider) || object == getParent())
			continue;
		
		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				if (Collision::checkCollision(circle, otherCircle->circle)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
			case LINE: {
				LineCollider* otherLine = static_cast<LineCollider*>(otherCollider);
				if (Collision::checkCollision(circle, otherLine->line)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
			case POINT: {
				PointCollider* otherPoint = static_cast<PointCollider*>(otherCollider);
				if (Collision::checkCollision(otherPoint->point, circle)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
		}
	}
}

void LineCollider::checkCollisions(const Scene& scene) {
	std::vector<GameObject*> closeObjects;
	try {
		// Get all GameObjects withing our bounding circle
		closeObjects = scene.getObjectTree().findObjectsInRange(line.start, checkRadius);
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || collisionList.count(otherCollider) || object == getParent())
			continue;
		
		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				if (Collision::checkCollision(otherCircle->circle, line)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
			case LINE: {
				LineCollider* otherLine = static_cast<LineCollider*>(otherCollider);
				if (Collision::checkCollision(line, otherLine->line)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
			case POINT: {
				// Currently no support for Line-Point collision
				break;
			}
		}
	}
}

void PointCollider::checkCollisions(const Scene& scene) {
	std::vector<GameObject*> closeObjects;
	try {
		// Get all GameObjects withing our bounding circle
		closeObjects = scene.getObjectTree().findObjectsInRange(point, checkRadius);
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || collisionList.count(otherCollider) || object == getParent())
			continue;
		
		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				if (Collision::checkCollision(point, otherCircle->circle)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
			case LINE: {
				// Currently no support for Line-Point collision
			}
			case POINT: {
				PointCollider* otherPoint = static_cast<PointCollider*>(otherCollider);
				if (Collision::checkCollision(point, otherPoint->point)) {
					addCollision(otherCollider);
					otherCollider->addCollision(this);
				}
				break;
			}
		}
	}
}
