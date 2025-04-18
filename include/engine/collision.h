#pragma once

#include "SDL2/SDL_rect.h"
#include <unordered_set>

#include "engine/vector2D.h"

class SDL_Renderer;
class GameObject;
class Scene;

namespace Collision {

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

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
bool checkCollision(const Circle& a, const Circle& b);
bool checkCollision(const vector2Df& point, const Circle& circle);
bool checkCollision(const Circle& circle, const Line& line);

void drawCircleCollider(SDL_Renderer* renderer, const Circle& collider);
}

class Collider {
public:
	Collider(const Collision::Types collisionType, const float checkRadius, GameObject* parent);
	Collider(const Collision::Types collisionType, const float checkRadius);

	void collisionUpdate();
	void addCollision(const Collider* other);
	virtual void checkCollisions(const Scene& scene) = 0;

	Collision::Types getCollisionType() const { return collisionType; }

	const GameObject* getParent() const { return parent; }

protected:
	/*
	* @abstract	Called when colliding with another collider.
	*			Default behavior is calling onCollision in parent.
	*			Override in deriving class to define custom behavior.
	* @param	other	Reference to the collider it is colliding with.
	*/
	virtual void onCollision(const Collider& other);

	std::unordered_set<const Collider*> collisionList;
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
