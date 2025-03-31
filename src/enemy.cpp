#include "enemy.h"
#include "game/game.h"
#include "game/collision.h"

Enemy::Enemy(const float& health, const float& speed, const float& steer, const float& sMult,
			 const float& slowing) :
			startHealth(health), moveSpeed(speed), maxSteer(steer), steerStrength(sMult),
			slowingRadius(slowing),
			healthbarBG(vector2Df(), vector2Df(75, 10), SDL_Color{ 255, 0, 0, 255 }),
			state{} {
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
	// Different movement depending on the current state
	vector2Df steering;
	switch (state) {
		case EnemyStates::PURSUIT:
			steering += pursuit(game->player, 0.75f);
			steering += seek(game->player->getPivotPosition()) * 0.75f;
			break;
		case EnemyStates::EVADE:
			steering += evade(game->player, 0.4f);
			break;
	}

	try {
		// Move away from closest enemy
		const vector2Df closest = game->getEnemyManager()->findClosestEnemy(pivotPosition);
		steering += flee(closest) * 0.7f;
	}
	catch (int e) {
		// Can't find closest enemy. Usually because there is currently only one enemy.
		// Does not require further action, hence why this catch is empty.
	}

	steering *= steerStrength; // Multiply steering by the steering strength
	// Clamp steering
	if (steering.getMagnitude() > maxSteer) {
		steering = steering.normalized() * maxSteer;
	}
	velocity += steering * deltaTime;

	// Clamp velocity
	if (velocity.getMagnitude() > moveSpeed) {
		velocity = velocity.normalized() * moveSpeed;
	}

	rotation = velocity.toDegrees() + 90; // Rotate enemy in direction of movement
	
	// Calculate animation speed based on movement speed
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
	// Evade player when health drops below 50
	else if (health <= 50) {
		state = EnemyStates::EVADE;
	}

	// Update healthbar
	healthbarSlider->setValue(health / startHealth * 100);
}

void Enemy::die() {
	// Death effects here
	deleteObject = true; // Delete object when dying
}

// Steering behaviors
vector2Df Enemy::seek(const vector2Df& target) const {
	const vector2Df targetDirection(target - pivotPosition); // Find target direction

	// Scale desiredVelocity to maximum speed
	vector2Df desiredVelocity = targetDirection.normalized() * moveSpeed;
	
	const float distance = targetDirection.getMagnitude();
	// Slow down if inside the slowing radius
	if (distance < slowingRadius) {
		desiredVelocity *= distance / slowingRadius;
	}

	return desiredVelocity - velocity; // Return calculated force
}

vector2Df Enemy::flee(const vector2Df& target) const {
	const vector2Df targetDirection(pivotPosition - target); // Find target direction

	// Scale desiredVelocity to maximum speed
	vector2Df desiredVelocity = targetDirection.normalized() * moveSpeed;

	return desiredVelocity - velocity; // Return calculated force
}

vector2Df Enemy::pursuit(const GameObject* target, const float& predictionMultiplier) const {
	const vector2Df distance = target->getPivotPosition() - pivotPosition;
	const float time = distance.getMagnitude() / moveSpeed;

	// Calculate the targets position in the future
	const vector2Df futurePosition = target->getPivotPosition() + target->getVelocity()
									* time * predictionMultiplier;
	return seek(futurePosition); // Use seek to move towards this position
}

vector2Df Enemy::evade(const GameObject* target, const float& predictionMultiplier) const {
	const vector2Df distance = target->getPivotPosition() - pivotPosition;
	const float time = distance.getMagnitude() / moveSpeed;

	// Calculate the targets position in the future
	const vector2Df futurePosition = target->getPivotPosition() + target->getVelocity()
									* time * predictionMultiplier;
	return flee(futurePosition); // Use seek to move towards this position
}
