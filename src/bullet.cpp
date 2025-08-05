#include "bullet.h"

#include "engine/collision.h"
#include "engine/gameObject.h"
#include "engine/scene.h"

Bullet::Bullet() : GameObject(Vec2(1, 2)) {
	setSize(Vec2{1.75f, 1.75f});

	collider = std::make_unique<LineCollider>(std::move(Collision::Line{}), 1000.0f, this);
	lineCollider = static_cast<LineCollider*>(collider.get());
}

void Bullet::initialize(const Scene& scene, const Vec2& startPos, const Vec2& direction,
                        const float rotation, const std::shared_ptr<GunData>& data) {
	GameObject::initialize(scene, startPos);  // Call base initialize
	velocity.x = direction.x * data->bulletSpeed;
	velocity.y = direction.y * data->bulletSpeed;
	this->rotation = rotation;
	this->data = data;

	lineCollider->line.start = lineCollider->line.end = position;
}

void Bullet::update(Scene& scene, const float deltaTime) {
	lineCollider->line.start = position;   // Change collision start before position update
	GameObject::update(scene, deltaTime);  // Update position
	lineCollider->line.end = position;     // Change collision end after position update

	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		// Delete bullet
		deleteObject = true;
	}
}

void Bullet::onCollision(const Collision::Event& event, Scene& scene) {
	// Delete object when colliding with something and stop further collision updates
	deleteObject = true;
	throw 1;
}
