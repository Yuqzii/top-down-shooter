#include "engine/scene.h"
#include "engine/collision.h"
#include "engine/game.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"

Enemy::Enemy(const float& health, const float& speed, const float& steer, const float& sMult,
			 const float& slowing) :
			startHealth(health), moveSpeed(speed), maxSteer(steer), steerStrength(sMult),
			slowingRadius(slowing),
			healthbarBG(vector2Df(), vector2Df(75, 10), SDL_Color{ 255, 0, 0, 255 }),
			state() {

	isAnimated = true;

	useCollision = true;
	circleCollider.radius = 220;

	healthbarSlider = new UI::Slider(SDL_Color { 0, 255, 0, 255 }, &healthbarBG);
}

void Enemy::initialize(const vector2Df& startPosition, const Scene& scene) {
	GameObject::initialize(startPosition, scene); // Call base initialize
	
	circleCollider.radius = 35;
	health = startHealth;

	// Initialize enemy at full speed towards player
	const vector2Df playerDirection = vector2Df(
		scene.player->getPivotPosition() - pivotPosition).normalized();
	velocity = playerDirection * moveSpeed;
}

void Enemy::update(Scene& scene, const float deltaTime) {
	// Different movement depending on the current state
	steering = vector2Df(); // Reset steering
	switch (state) {
		case EnemyStates::PURSUIT:
			steering += pursuit(scene.player, 0.75f);
			steering += seek(scene.player->getPivotPosition()) * 0.75f;
			break;
		case EnemyStates::EVADE:
			steering += evade(scene.player, 0.4f);
			break;
	}

	try {
		// Move away from closest enemy
		const Enemy* closest = scene.getEnemyManager().findClosestEnemy(pivotPosition);
		steering += flee(closest->getPivotPosition()) * 0.7f;
	}
	catch (int e) {
		// Can't find closest enemy. Usually because there is currently only one enemy.
		// Does not require further action, hence why this catch is empty.
	}

	steering *= steerStrength; // Multiply steering by the steering strength
	// Clamp steering
	steering = steering.clamped(maxSteer);
	velocity += steering * deltaTime;

	// Clamp velocity
	velocity = velocity.clamped(moveSpeed);

	rotation = velocity.toDegrees() + 90; // Rotate enemy in direction of movement
	
	// Calculate animation speed based on movement speed
	animationSpeed = velocity.magnitude() / moveSpeed;

	GameObject::update(scene, deltaTime); // Update position

	// Checks if colliding with player
	if (Collision::checkCollision(circleCollider, scene.player->circleCollider)) {
		// Damage player or something
	}
	
	// Update healthbar
	if (health < startHealth) {
		healthbarBG.localPosition.x = pivotPosition.x - healthbarBG.localSize.x / 2;
		healthbarBG.localPosition.y = pivotPosition.y + 60;
		healthbarBG.calculatePosition();
		healthbarBG.update();
		// Render healthbar
		scene.getGameInstance().getRenderManager().addRenderCall(
				healthbarBG.getRenderFunction(), this);
	}
}

void Enemy::onCollision(const GameObject* other) {
	const Bullet* bullet = dynamic_cast<const Bullet*>(other);
	if (bullet == nullptr) return; // Return if colliding with something that is not a bullet
	
	takeDamage(bullet->getDamage());
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
	
	const float distance = targetDirection.magnitude();
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
	const float time = distance.magnitude() / moveSpeed;

	// Calculate the targets position in the future
	const vector2Df futurePosition = target->getPivotPosition() + target->getVelocity()
									* time * predictionMultiplier;
	return seek(futurePosition); // Use seek to move towards this position
}

vector2Df Enemy::evade(const GameObject* target, const float& predictionMultiplier) const {
	const vector2Df distance = target->getPivotPosition() - pivotPosition;
	const float time = distance.magnitude() / moveSpeed;

	// Calculate the targets position in the future
	const vector2Df futurePosition = target->getPivotPosition() + target->getVelocity()
									* time * predictionMultiplier;
	return flee(futurePosition); // Use seek to move towards this position
}

std::function<void(SDL_Renderer*)> Enemy::debugRender() const {
	return [this](SDL_Renderer* renderer) {
		GameObject::debugRender()(renderer); // Call parent debugRender and pass in renderer

		// Draw line displaying steering direction and strength
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderDrawLine(renderer, pivotPosition.x, pivotPosition.y,
					 pivotPosition.x + steering.x * 0.1,
					 pivotPosition.y + steering.y * 0.1
		);
	};
}
