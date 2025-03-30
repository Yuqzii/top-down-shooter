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

	// Loop through enemies and check for collision
	for (Enemy* enemy : game->getEnemyManager().getEnemies()) {
		if (Collision::checkCollision(circleCollider, enemy->circleCollider)) {
			// Bullet collided with enemy
			enemy->takeDamage(damage); // Damage enemy
			deleteObject = true; // Delete bullet
			break; // Stop checking for collision so that one bullet only damages one enemy
		}
	}
}

void Bullet::initializeDirection(const vector2Df direction, const float rotation) {
	velocity.x = direction.x * speed;
	velocity.y = direction.y * speed;
	this->rotation = rotation;
}
