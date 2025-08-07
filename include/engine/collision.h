#pragma once

#include <unordered_set>
#include <vector>

#include "SDL2/SDL_rect.h"
#include "engine/vector2D.h"

struct SDL_Renderer;
class GameObject;
class Scene;
class Collider;

namespace Collision {

struct Event {
	const bool collided;
	const float depth;
	const Vec2 position;
	const Collider* other;

	Event(const bool collided_, const float depth_, const Vec2& position_, const Collider* other_)
	    : collided{collided_}, depth{depth_}, position{position_}, other{other_} {}
	Event(const bool collided_, const float depth_, const Vec2& position_)
	    : Event{collided_, depth_, position_, nullptr} {}
	Event(const bool collided_, const Vec2& position_) : Event{collided_, 0, position_} {}
	Event(const bool collided_, const float depth_) : Event{collided_, depth_, Vec2{}} {}
	Event(const bool collided_) : Event{collided_, 0} {}
};

enum class Types {
	CIRCLE = 0,
	LINE,
	POINT,
};

struct Circle {
	Vec2 position;
	float radius;

	Circle(const Vec2& pos, const float r) : position{pos}, radius{r} {}
	Circle(const float r) : Circle{Vec2(), r} {}
	Circle() : Circle{Vec2(), 0.0f} {}
};

struct Line {
	Vec2 position;
	Vec2 start;
	Vec2 end;

	Line() : start(), end() {}
	Line(const Vec2& s, const Vec2& e) : start{s}, end{e}, position{s + (e - s) * 0.5f} {}
	Line(const Vec2& pos, const Vec2& s, const Vec2& e) : start{s}, end{e}, position{pos} {}
};

Vec2 closestPointOnLine(const Vec2& point, const Line& line);

Collision::Event checkCollision(const SDL_Rect& a, const SDL_Rect& b);
Collision::Event checkCollision(const Circle& a, const Circle& b);
Collision::Event checkCollision(const Vec2& point, const Circle& circle);
Collision::Event checkCollision(const Circle& circle, const Line& line);

/*
 * @abstract	Calculates the necessary movement after a collision with a static LineCollider
 *				to resolve the collision.
 * @param	event	Reference to the collision event
 * @param	position	Position of the collider that collided with the static LineCollider.
 *						This is what we will calculate the movement for.
 * @return	Movement vector. Apply this to the position to resolve collision.
 */
Vec2 resolveStaticLine(const Collision::Event& event, const Vec2& position);

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider);
}  // namespace Collision

class Collider {
public:
	Collider(const Collision::Types collisionType, const bool isStatic, GameObject* parent);
	Collider(const Collision::Types collisionType, const float checkRadius, GameObject* parent);
	Collider(const Collision::Types collisionType, const float checkRadius);
	virtual ~Collider() = default;

	void collisionUpdate(Scene& scene);
	void addCollision(const Collision::Event event);
	virtual void checkCollisions(const Scene& scene) = 0;

	Collision::Types getCollisionType() const { return collisionType; }

	const GameObject* getParent() const { return parent; }
	float getCheckRadius() const { return checkRadius; }
	bool getIsStatic() const { return isStatic; }

	virtual std::string_view getTag() const {
		return "";
	}

protected:
	/*
	 * @abstract	Called when colliding with another collider.
	 *			Default behavior is calling onCollision in parent.
	 *			Override in deriving class to define custom behavior.
	 * @param	event	Reference to the collision event that occured.
	 */
	virtual void onCollision(const Collision::Event& event, Scene& scene);

	std::unordered_set<const Collider*> haveCollidedWith;
	std::vector<Collision::Event> collisionEvents;

private:
	const float checkRadius;
	const bool isStatic;

private:
	Collision::Types collisionType;

	GameObject* parent;
};

class CircleCollider : public Collider {
public:
	CircleCollider(Collision::Circle collider, const bool isStatic, GameObject* parent);
	CircleCollider(Collision::Circle collider, const float checkRadius, GameObject* parent);
	CircleCollider(Collision::Circle collider, const float checkRadius);

	Collision::Circle circle;

	void checkCollisions(const Scene& scene) override;
};

class LineCollider : public Collider {
public:
	LineCollider(Collision::Line collider, const bool isStatic, GameObject* parent);
	LineCollider(Collision::Line collider, const float checkRadius, GameObject* parent);
	LineCollider(Collision::Line collider, const float checkRadius);

	Collision::Line line;

	void checkCollisions(const Scene& scene) override;
};

class PointCollider : public Collider {
public:
	PointCollider(Vec2 point, const bool isStatic, GameObject* parent);
	PointCollider(Vec2 point, const float checkRadius, GameObject* parent);
	PointCollider(Vec2 point, const float checkRadius);

	Vec2 point;

	void checkCollisions(const Scene& scene) override;
};
