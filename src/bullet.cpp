#include "bullet.h"
#include "enemy.h"
#include "engine/gameObject.h"
#include "engine/game.h"
#include "engine/collision.h"

void Bullet::initialize(const vector2Df& position, Game* game) {
	GameObject::initialize(position, game); // Call base initialize
	
	previousPosition = pivotPosition;
}

void Bullet::update(Game* game, const double& deltaTime) {
	previousPosition = pivotPosition;
	GameObject::update(game, deltaTime); // Update position

	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		// Delete bullet
		deleteObject = true;
	}
}

void Bullet::checkCollisions(Game* game) {
	// Get all GameObjects withing bounding circle
	std::vector<GameObject*> closeObjects =
			game->getObjectTree().findObjectsInRange(pivotPosition, boundingCircle);

	const Collision::Line movementLine(previousPosition, pivotPosition);

	for (GameObject* object : closeObjects) {
		if (!object->getUseCollision() || collisionList.count(object) || object == this)
			continue;

		// Check if bullet travelled through any of the objects
		if (Collision::checkCollision(object->circleCollider, movementLine)) {
			addCollision(object);
			object->addCollision(this);
			break; // Bullet is deleted upon collision anyways, no point in checking more
		}
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
