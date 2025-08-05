#include "engine/collision.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "SDL2/SDL_render.h"
#include "engine/gameObject.h"
#include "engine/scene.h"

namespace Collision {

namespace {
int distanceSquared(const Vec2& a, const Vec2& b) {
	int deltaX = b.x - a.x;
	int deltaY = b.y - a.y;
	return deltaX * deltaX + deltaY * deltaY;
}

int roundUpToMultipleOfEight(int x) { return (x + (8 - 1)) & -8; }

// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool onSegment(const Vec2& p, const Vec2& q, const Vec2& r) {
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) &&
	    q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
int orientation(const Vec2& p, const Vec2& q, const Vec2& r) {
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	const int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // collinear

	return (val > 0) ? 1 : 2;  // clock or counterclock wise
}

}  // namespace

Vec2 closestPointOnLine(const Vec2& point, const Line& line) {
	Vec2 tangent = line.end - line.start;

	if ((point - line.start).dotProduct(tangent) <= 0) {
		return line.start;  // point is at or before start of line
	}

	if ((point - line.end).dotProduct(tangent) >= 0) {
		return line.end;  // point is at after end of line
	}

	tangent = tangent.normalized();
	const Vec2 relativePos = point - line.start;
	return line.start + tangent * (tangent.dotProduct(relativePos));
}

// Returns true if rect a overlaps with rect b
// Not used
Collision::Event checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.y + a.h <= b.y || a.y >= b.y + b.h || a.x + a.w <= b.x || a.x >= b.x + b.w) {
		return false;
	} else {
		return true;
	}
}

Collision::Event checkCollision(const Circle& a, const Circle& b) {
	// We can use distance squared instead of just distance to optimize,
	// because getting the normal distance requires square root (an expensive operation),
	// but if x > y then x^2 > y^2, so we can square both values and compare instead
	int totalRadiusSquared = (a.radius + b.radius);
	totalRadiusSquared *= totalRadiusSquared;

	const float depth = distanceSquared(a.position, b.position) - totalRadiusSquared;
	// If distance between center of circles is less than sum of their radii
	if (depth < 0) {
		return Event{true, depth * -1};
	}
	return Event{false};
}

Collision::Event checkCollision(const Vec2& point, const Circle& c) {
	const float dist = distanceSquared(point, c.position);
	if (dist < c.radius * c.radius) {
		// Point is inside circle
		const Vec2 delta = c.position - point;
		const float depth = c.radius - delta.magnitude();
		return Event{true, depth, point};
	} else
		return Event{false};
}

Collision::Event checkCollision(const Circle& c, const Line& l) {
	// Are any of the endpoints inside the circle?
	const Event sInside = checkCollision(l.start, c);
	if (sInside.collided) return std::move(sInside);
	const Event eInside = checkCollision(l.end, c);
	if (eInside.collided) return std::move(eInside);

	const Vec2 closestPoint = closestPointOnLine(c.position, l);
	const Vec2 delta = c.position - closestPoint;
	// Is distance from closest point to circle larger than its radius?
	if (delta.dotProduct(delta) < c.radius * c.radius) {
		const float depth = c.radius - delta.magnitude();
		return Event{true, depth, closestPoint};
	} else
		return Event{false};
}

// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
Collision::Event checkCollision(const Line& a, const Line& b) {
	const int o1 = orientation(a.start, a.end, b.start);
	const int o2 = orientation(a.start, a.end, b.end);
	const int o3 = orientation(b.start, b.end, a.start);
	const int o4 = orientation(b.start, b.end, a.end);

	if (o1 != o2 && o3 != o4) {
		// Source: https://www.geeksforgeeks.org/line-intersection-in-cpp/
		const float a1 = a.end.y - a.start.y;
		const float b1 = a.start.x - a.end.x;
		const float c1 = a1 * a.start.x + b1 * a.start.y;

		const float a2 = b.end.y - b.start.y;
		const float b2 = b.start.x - b.end.x;
		const float c2 = a2 * b.start.x + b2 * b.start.y;

		const float determinant = a1 * b2 - a2 * b1;
		Vec2 res{};
		if (determinant != 0) {
			res.x = (c1 * b2 - c2 * b1) / determinant;
			res.y = (a1 * c2 - a2 * c1) / determinant;
		}

		return Event{true, res};
	}

	// Special Cases
	// a.start, a.end and b.start are collinear and b.start lies on segment a
	if (o1 == 0 && onSegment(a.start, b.start, a.end)) return Event{true};

	// a.start, a.end and b.end are collinear and b.end lies on segment a
	if (o2 == 0 && onSegment(a.start, b.end, a.end)) return Event{true};

	// b.start, b.end and a.start are collinear and a.start lies on segment b
	if (o3 == 0 && onSegment(b.start, a.start, b.end)) return Event{true};

	// b.start, b.end and a.end are collinear and a.end lies on segment b
	if (o4 == 0 && onSegment(b.start, a.end, b.end)) return Event{true};

	return Event{false};  // Doesn't fall in any of the above cases
}

Collision::Event checkCollision(const Vec2& a, const Vec2& b) { return Event{a == b}; }

Vec2 resolveStaticLine(const Collision::Event& event, const Vec2& position) {
	const LineCollider& line = static_cast<const LineCollider&>(*event.other);
	// Find the normal direction of the line
	Vec2 normal = Vec2{line.line.start - line.line.end}.normalized();
	normal = Vec2{normal.y, normal.x * -1};
	// Check what side of line position is on
	const float angle = std::acos(normal.normalized().dotProduct(
	    (position - event.other->getParent()->getPosition()).normalized()));
	const float degrees = angle * 180 / M_PI;
	// Calculate movement according to collision depth and what side position is on
	if (degrees >= 90.0f && degrees <= 180.0f)
		return normal * event.depth * -1.0f;
	else
		return normal * event.depth;
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

}  // namespace Collision

Collider::Collider(const Collision::Types collisionType_, const bool isStatic_, GameObject* parent_)
    : collisionType{collisionType_}, checkRadius{0.0f}, isStatic{isStatic_}, parent{parent_} {
	if (!isStatic_) {
		std::cout << "Collider used static constructor, but is not marked static.\n";
	}
}

Collider::Collider(const Collision::Types collisionType_, const float checkRadius_,
                   GameObject* parent_)
    : collisionType{collisionType_}, checkRadius{checkRadius_}, isStatic{false}, parent{parent_} {}

Collider::Collider(const Collision::Types collisionType_, const float checkRadius_)
    : Collider{collisionType_, checkRadius_, nullptr} {}

CircleCollider::CircleCollider(Collision::Circle circle_, const bool isStatic_, GameObject* parent)
    : circle{std::move(circle_)}, Collider{Collision::Types::CIRCLE, isStatic_, parent} {}

CircleCollider::CircleCollider(Collision::Circle circle_, const float checkRadius_,
                               GameObject* parent_)
    : circle{std::move(circle_)}, Collider{Collision::Types::CIRCLE, checkRadius_, parent_} {}

CircleCollider::CircleCollider(Collision::Circle circle_, const float checkRadius_)
    : CircleCollider{std::move(circle_), checkRadius_, nullptr} {}

LineCollider::LineCollider(Collision::Line line_, const bool isStatic_, GameObject* parent)
    : line{std::move(line_)}, Collider{Collision::Types::LINE, isStatic_, parent} {}

LineCollider::LineCollider(Collision::Line line_, const float checkRadius_, GameObject* parent)
    : line{line_}, Collider{Collision::Types::LINE, checkRadius_, parent} {}

LineCollider::LineCollider(Collision::Line line_, const float checkRadius_)
    : LineCollider{std::move(line_), checkRadius_, nullptr} {}

PointCollider::PointCollider(Vec2 point_, const bool isStatic_, GameObject* parent)
    : point{std::move(point_)}, Collider{Collision::Types::POINT, isStatic_, parent} {}

PointCollider::PointCollider(Vec2 point_, const float checkRadius_, GameObject* parent)
    : point{std::move(point_)}, Collider{Collision::Types::POINT, checkRadius_, parent} {}

PointCollider::PointCollider(Vec2 point_, const float checkRadius_)
    : PointCollider{std::move(point_), checkRadius_, nullptr} {}

void Collider::collisionUpdate(Scene& scene) {
	for (const Collision::Event& event : collisionEvents) {
		try {
			onCollision(event, scene);
		} catch (int e) {
			// Stop collision detection when throwing exception
			break;
		}
	}

	// Make sure only collisions from one frame are registered
	haveCollidedWith.clear();
	collisionEvents.clear();
}

void Collider::addCollision(const Collision::Event event) {
	if (!haveCollidedWith.count(event.other)) {
		haveCollidedWith.insert(event.other);
		collisionEvents.push_back(event);
	}
}

void Collider::onCollision(const Collision::Event& event, Scene& scene) {
	if (parent == nullptr) return;
	parent->onCollision(event, scene);
}

void CircleCollider::checkCollisions(const Scene& scene) {
	std::vector<GameObject*> closeObjects;
	try {
		// Get all GameObjects withing our bounding circle
		closeObjects = scene.getObjectTree().findObjectsInRange(circle.position, getCheckRadius());
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || haveCollidedWith.count(otherCollider) ||
		    object == getParent())
			continue;

		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(circle, otherCircle->circle);
				if (event.collided) {
					event.other = otherCircle;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
				}
				break;
			}
			case LINE: {
				LineCollider* otherLine = static_cast<LineCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(circle, otherLine->line);
				if (event.collided) {
					event.other = otherLine;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
				}
				break;
			}
			case POINT: {
				PointCollider* otherPoint = static_cast<PointCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(otherPoint->point, circle);
				if (event.collided) {
					event.other = otherPoint;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
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
		closeObjects = scene.getObjectTree().findObjectsInRange(line.start, getCheckRadius());
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || haveCollidedWith.count(otherCollider) ||
		    object == getParent())
			continue;

		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(otherCircle->circle, line);
				if (event.collided) {
					event.other = otherCircle;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
				}
				break;
			}
			case LINE: {
				LineCollider* otherLine = static_cast<LineCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(line, otherLine->line);
				if (event.collided) {
					event.other = otherLine;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
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
		closeObjects = scene.getObjectTree().findObjectsInRange(point, getCheckRadius());
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
		return;
	}

	for (GameObject* object : closeObjects) {
		Collider* otherCollider = object->getCollider();
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (otherCollider == nullptr || haveCollidedWith.count(otherCollider) ||
		    object == getParent())
			continue;

		switch (otherCollider->getCollisionType()) {
			using enum Collision::Types;

			case CIRCLE: {
				CircleCollider* otherCircle = static_cast<CircleCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(point, otherCircle->circle);
				if (event.collided) {
					event.other = otherCircle;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
				}
				break;
			}
			case LINE: {
				// Currently no support for Line-Point collision
			}
			case POINT: {
				PointCollider* otherPoint = static_cast<PointCollider*>(otherCollider);
				Collision::Event event = Collision::checkCollision(point, otherPoint->point);
				if (event.collided) {
					event.other = otherPoint;
					addCollision(event);
					event.other = this;
					otherCollider->addCollision(event);
				}
				break;
			}
		}
	}
}
