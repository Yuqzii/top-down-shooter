#include "bullet.h"
#include "enemy.h"
#include "game/game.h"
#include "game/collision.h"

void Bullet::initialize(const std::string& textureSheet, const vector2Df& position, Game* game) {
	GameObject::initialize(textureSheet, position, game); // Call base initialize
	
	circleCollider.radius = 15; // Change collider size
}

void Bullet::update(Game* game, const double& deltaTime) {
	GameObject::update(game, deltaTime); // Call base update

	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;

	timeLeft -= deltaTime;
	if (timeLeft <= 0) {
		// Delete bullet
		deleteObject = true;
	}

	// Loop through enemies and check for collision
	for (Enemy* enemy : game->getEnemySpawner().getEnemies()) {
		if (Collision::checkCollision(circleCollider, enemy->circleCollider)) {
			// Bullet collided with enemy
			enemy->deleteObject = true; // Delete hit enemy
			deleteObject = true; // Delete bullet
		}
	}
}

void Bullet::initializeDirection(const vector2Df direction, const float rotation) {
	this->direction = direction;
	this->rotation = rotation;
}
