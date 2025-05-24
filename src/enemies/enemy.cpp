#include "enemies/enemy.h"

#include "bullet.h"
#include "engine/collision.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "player.h"
#include "scenes/combat_scene.h"
#include "terrain/terrainCollider.h"

Enemy::Enemy(const float health_, const float damage_, const float speed, const float steer,
			 const float sMult, const float slowing)
	: startHealth(health_),
	  damage(damage_),
	  health(health_),
	  moveSpeed(speed),
	  maxSteer(steer),
	  steerStrength(sMult),
	  slowingRadius(slowing),
	  isMoving(true),
	  healthbarBG(Vec2(), Vec2(75, 10), SDL_Color{255, 0, 0, 255}),
	  state(),
	  GameObject{
		  std::make_unique<CircleCollider>(std::move(Collision::Circle{40.0f}), 500.0f, this)},
	  circleCollider{static_cast<CircleCollider&>(*collider)} {
	healthbarSlider = new UI::Slider(SDL_Color{0, 255, 0, 255}, &healthbarBG);
}

void Enemy::initialize(const Scene& scene, const Vec2& startPos) {
	GameObject::initialize(scene, startPos);  // Call base initialize

	// Store Scene as CombatScene to avoid unnecessary casts at update
	combatScene = static_cast<const CombatScene*>(&scene);

	// Initialize enemy at full speed towards player
	const Vec2 playerDirection = Vec2(combatScene->player.getPosition() - position).normalized();
	velocity = playerDirection * moveSpeed;
}

void Enemy::update(Scene& scene, const float deltaTime) {
	if (isMoving) {
		steering *= steerStrength;	// Multiply steering by the steering strength
		// Clamp steering
		steering = steering.clamped(maxSteer);
		velocity += steering * deltaTime;

		// Clamp velocity
		velocity = velocity.clamped(moveSpeed);

		rotation = velocity.toDegrees() + 90;  // Rotate enemy in direction of movement

		// Calculate animation speed based on movement speed
		animationSpeed = velocity.magnitude() / moveSpeed;
	} else
		velocity = Vec2();

	GameObject::update(scene, deltaTime);		// Update position
	circleCollider.circle.position = position;	// Update collider position

	// Update healthbar
	if (health < startHealth) {
		healthbarBG.localPosition.x = getScreenPosition().x - healthbarBG.localSize.x / 2;
		healthbarBG.localPosition.y = getScreenPosition().y + 60;
		healthbarBG.calculatePosition();
		healthbarBG.update();
		// Render healthbar
		scene.getGame().getRenderManager().addRenderCall(healthbarBG.getRenderFunction(), this);
	}
}

void Enemy::onCollision(const Collision::Event& event, Scene& scene) {
	const Bullet* bullet = dynamic_cast<const Bullet*>(event.other->getParent());
	if (bullet) {
		takeDamage(bullet->getData().damage);
		return;
	}

	const TerrainCollider* terrainCollider =
		dynamic_cast<const TerrainCollider*>(event.other->getParent());
	if (terrainCollider) {
		position += Collision::resolveStaticLine(event, position);
		return;
	}
}

void Enemy::takeDamage(const float damage) {
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
	deleteObject = true;  // Delete object when dying
}

// Steering behaviors
Vec2 Enemy::seek(const Vec2& target) const {
	const Vec2 targetDirection(target - position);	// Find target direction

	// Scale desiredVelocity to maximum speed
	Vec2 desiredVelocity = targetDirection.normalized() * moveSpeed;

	const float distance = targetDirection.magnitude();
	// Slow down if inside the slowing radius
	if (distance < slowingRadius) {
		desiredVelocity *= distance / slowingRadius;
	}

	return desiredVelocity - velocity;	// Return calculated force
}

Vec2 Enemy::flee(const Vec2& target) const {
	const Vec2 targetDirection(position - target);	// Find target direction

	// Scale desiredVelocity to maximum speed
	Vec2 desiredVelocity = targetDirection.normalized() * moveSpeed;

	return desiredVelocity - velocity;	// Return calculated force
}

Vec2 Enemy::pursuit(const GameObject& target, const float& predictionMultiplier) const {
	const Vec2 distance = target.getPosition() - position;
	const float time = distance.magnitude() / moveSpeed;

	// Calculate the targets position in the future
	const Vec2 futurePosition =
		target.getPosition() + target.getVelocity() * time * predictionMultiplier;
	return seek(futurePosition);  // Use seek to move towards this position
}

Vec2 Enemy::evade(const GameObject& target, const float& predictionMultiplier) const {
	const Vec2 distance = target.getPosition() - position;
	const float time = distance.magnitude() / moveSpeed;

	// Calculate the targets position in the future
	const Vec2 futurePosition =
		target.getPosition() + target.getVelocity() * time * predictionMultiplier;
	return flee(futurePosition);  // Use seek to move towards this position
}

void Enemy::avoidTerrain(const float strength, const float avoidDist) {
	try {
		// Find closest terrain object
		const GameObject* closest =
			combatScene->getTerrainManager().getTree().findClosestObject(position);
		// Find the closest point to this enemy on the LineCollider
		const Collision::Line& line = static_cast<LineCollider*>(closest->getCollider())->line;
		const Vec2 closestPoint = Collision::closestPointOnLine(position, line);

		const Vec2 dist{closestPoint - position};  // Find distance to the closest point
		if (dist.dotProduct(dist) <= avoidDist * avoidDist) {
			// Avoid the middle position of the collider
			steering += flee(closest->getPosition()) * strength;
		}
	} catch (const int e) {
		// Could not get the closest object from the tree, likely due to it being empty.
		// Does not require further action, hence this catch block is empty.
	}
}

std::function<void(Scene&)> Enemy::debugRender() const {
	return [this](Scene& scene) {
		GameObject::debugRender()(scene);  // Call parent debugRender

		SDL_Renderer* renderer = scene.getGame().getRenderer();
		// Draw line displaying steering direction and strength
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderDrawLine(renderer, getScreenPosition().x, getScreenPosition().y,
						   getScreenPosition().x + steering.x * 0.1,
						   getScreenPosition().y + steering.y * 0.1);
	};
}

EnemyAttackPoint::EnemyAttackPoint() {
	collider = std::make_unique<CircleCollider>(std::move(Collision::Circle{10.0f}), 100.0f, this);
	renderObject = false;
}

void EnemyAttackPoint::initialize(const Scene& scene, const Vec2& position, const Enemy* parent) {
	GameObject::initialize(scene, position);
	this->parent = parent;
	static_cast<CircleCollider*>(collider.get())->circle.position = position;
	deleteObject = true;
}
