#include "enemy.h"
#include "game/game.h"
#include "game/collision.h"

void Enemy::initialize(const std::string& textureSheet,
					   const vector2Df& startPosition, Game* game) {
	GameObject::initialize(textureSheet, startPosition, game); // Call base initialize
	
	circleCollider.radius = 35;
}

void Enemy::update(Game* game, const double& deltaTime) {
	GameObject::update(game, deltaTime);

	// Get direction towards player
	vector2Df playerDirection(game->player->position.x - position.x,
							game->player->position.y - position.y);
	playerDirection.normalize();
	rotation = playerDirection.toDegrees() + 90; // Rotate enemy towards player
	
	// Move enemy towards player
	position.x += playerDirection.x * moveSpeed * deltaTime;
	position.y += playerDirection.y * moveSpeed * deltaTime;

	// Checks if colliding with player
	if (Collision::checkCollision(circleCollider, game->player->circleCollider)) {
		// Damage player or something
	}
}
