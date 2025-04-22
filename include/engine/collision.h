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
	const vector2Df position;
	const Collider* other;

	Event(const bool collided_, const float depth_, const vector2Df& position_,
		  const Collider* other_)
		: collided{collided_}, depth{depth_}, position{position_}, other{other_} {}
	Event(const bool collided_, const float depth_, const vector2Df& position_)
		: Event{collided_, depth_, position_, nullptr} {}
	Event(const bool collided_, const vector2Df& position_) : Event{collided_, 0, position_} {}
	Event(const bool collided_, const float depth_) : Event{collided_, depth_, vector2Df{}} {}
	Event(const bool collided_) : Event{collided_, 0} {}
};

enum class Types {
	CIRCLE = 0,
	LINE,
	POINT,
};

struct Circle {
	vector2Df position;
	float radius;

	Circle(const vector2Df& pos, const float r) : position{pos}, radius{r} {}
	Circle(const float r) : Circle{vector2Df(), r} {}
	Circle() : Circle{vector2Df(), 0.0f} {}
};

struct Line {
	vector2Df start;
	vector2Df end;

	Line() : start(), end() {}
	Line(const vector2Df& s, const vector2Df& e) : start(s), end(e) {}
};

vector2Df closestPointOnLine(const vector2Df& point, const Line& line);

Collision::Event checkCollision(const SDL_Rect& a, const SDL_Rect& b);
Collision::Event checkCollision(const Circle& a, const Circle& b);
Collision::Event checkCollision(const vector2Df& point, const Circle& circle);
Collision::Event checkCollision(const Circle& circle, const Line& line);

/*
 * @abstract	Calculates the necessary movement after a collision with a static LineCollider
 *				to resolve the collision.
 * @param	event	Reference to the collision event
 * @param	position	Position of the collider that collided with the static LineCollider.
 *						This is what we will calculate the movement for.
 * @return	Movement vector. Apply this to the position to resolve collision.
 */
vector2Df resolveStaticLine(const Collision::Event& event, const vector2Df& position);

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider);
}  // namespace Collision

class Collider {
public:
	Collider(const Collision::Types collisionType, const float checkRadius, GameObject* parent);
	Collider(const Collision::Types collisionType, const float checkRadius);

	void collisionUpdate(Scene& scene);
	void addCollision(const Collision::Event event);
	virtual void checkCollisions(const Scene& scene) = 0;

	Collision::Types getCollisionType() const { return collisionType; }

	const GameObject* getParent() const { return parent; }

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
	const float checkRadius;

private:
	Collision::Types collisionType;

	GameObject* parent;
};

class CircleCollider : public Collider {
public:
	CircleCollider(Collision::Circle collider, const float checkRadius, GameObject* parent);
	CircleCollider(Collision::Circle collider, const float checkRadius);

	Collision::Circle circle;

	void checkCollisions(const Scene& scene) override;
};

class LineCollider : public Collider {
public:
	LineCollider(Collision::Line collider, const float checkRadius, GameObject* parent);
	LineCollider(Collision::Line collider, const float checkRadius);

	Collision::Line line;

	void checkCollisions(const Scene& scene) override;
};

class PointCollider : public Collider {
public:
	PointCollider(vector2Df point, const float checkRadius, GameObject* parent);
	PointCollider(vector2Df point, const float checkRadius);

	vector2Df point;

	void checkCollisions(const Scene& scene) override;
};
