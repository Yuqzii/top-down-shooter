#include "bullet.h"
#include "game/game.h"

void Bullet::update(Game* game, const double& deltaTime) {
	GameObject::update(game, deltaTime);

	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;

	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		// Delete bullet
		deleteObject = true;
	}
}

void Bullet::initializeDirection(const vector2Df direction, const float rotation) {
	this->direction = direction;
	this->rotation = rotation;
}
