#include "enemy.h"
#include "game/game.h"
#include "game/collision.h"

Enemy::Enemy() : healthbarBG(vector2Df(), vector2Df(75, 10), SDL_Color{ 255, 0, 0, 255 }) {
	isAnimated = true;

	healthbarSlider = new UI::Slider(SDL_Color { 0, 255, 0, 255 }, &healthbarBG);
}

void Enemy::initialize(const vector2Df& startPosition, Game* game) {
	GameObject::initialize(startPosition, game); // Call base initialize
	
	circleCollider.radius = 35;
	health = startHealth;

	// Initialize enemy at full speed towards player
	const vector2Df playerDirection = vector2Df(
		game->player->getPivotPosition() - pivotPosition).normalized();
	velocity = playerDirection * moveSpeed;
}

void Enemy::update(Game* game, const double& deltaTime) {
	// Get direction towards player
	const vector2Df playerDirection = vector2Df(
		game->player->getPivotPosition() - pivotPosition).normalized();
	
	const vector2Df desiredVelocity = playerDirection * moveSpeed; // Scale desiredVelocity to max
	vector2Df steering = desiredVelocity - velocity; // Calculate steering
	
	// Limit steering magnitude to moveSpeed
	if (steering.getMagnitude() > maxSteer) {
		steering = steering.normalized() * maxSteer;
	}

	velocity += steering; // Add steering velocity
	// Limit velocity to moveSpeed
	if (velocity.getMagnitude() > moveSpeed) {
		velocity = velocity.normalized() * moveSpeed;
	}

	rotation = velocity.toDegrees() + 90; // Rotate enemy in direction of movement

	animationSpeed = velocity.getMagnitude() / moveSpeed;

	GameObject::update(game, deltaTime); // Update position

	// Checks if colliding with player
	if (Collision::checkCollision(circleCollider, game->player->circleCollider)) {
		// Damage player or something
	}
	
	// Update healthbar
	if (health < startHealth) {
		healthbarBG.localPosition.x = pivotPosition.x - healthbarBG.localSize.x / 2;
		healthbarBG.localPosition.y = pivotPosition.y + 60;
		healthbarBG.calculatePosition();
		healthbarBG.update();
		game->getUIManager()->addRenderCall(healthbarBG.getRenderFunction()); // Render healthbar
	}
}

void Enemy::takeDamage(const float& damage) {
	health -= damage;
	
	if (health <= 0) {
		die();
	}

	// Update healthbar
	healthbarSlider->setValue(health / startHealth * 100);
}

void Enemy::die() {
	// Death effects here
	deleteObject = true; // Delete object when dying
}
