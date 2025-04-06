#include "bullet.h"
#include "enemy.h"
#include "game/game.h"
#include "game/collision.h"

void Bullet::initialize(const vector2Df& position, Game* game) {
	GameObject::initialize(position, game); // Call base initialize
	
	circleCollider.radius = 15; // Change collider size
}

void Bullet::update(Game* game, const double& deltaTime) {
	GameObject::update(game, deltaTime); // Update position

	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		// Delete bullet
		deleteObject = true;
	}
}

void Bullet::onCollision(const GameObject* other) {
	// Delete object when colliding with something and stop further collision updates
	deleteObject = true;
	throw 1;
}

void Bullet::initializeDirection(const vector2Df direction, const float rotation) {
	velocity.x = direction.x * speed;
	velocity.y = direction.y * speed;
	this->rotation = rotation;
}
