#include "enemies/spider.h"

#include "engine/game.h"
#include "player.h"
#include "scenes/combat_scene.h"

SpiderEnemy::SpiderEnemy(const float startHealth, const float damage, const float moveSpeed,
						 const float maxSteer, const float steerMult, const float slowingRadius)
	: Enemy(startHealth, damage, moveSpeed, maxSteer, steerMult, slowingRadius) {
	isAnimated = true;
	circleCollider.circle.radius = 50;

	animationEvents.emplace_back(1, 6, [this](Scene& scene) { attack(scene); });
}

void SpiderEnemy::update(Scene& scene, const float deltaTime) {
	steering = Vec2();	// Reset steering
	// Different movement depending on the current state
	switch (getState()) {
		using enum EnemyStates;
		case PURSUIT:
			steering += pursuit(combatScene->player, 0.75f);
			steering += seek(combatScene->player.getPosition()) * 0.75f;
			avoidOtherEnemies(0.5f);
			attackRangeCheck();
			break;

		case EVADE:
			steering += evade(combatScene->player, 0.4f);
			avoidOtherEnemies();
			attackRangeCheck();
			break;

		case ATTACK:
			break;

		case REPOSITION:
			steering += flee(combatScene->player.getPosition()) * 1.75f;
			avoidOtherEnemies(0.3f);

			// Change to pursuit after repositionTime has passed
			repositionTimer -= deltaTime;
			if (repositionTimer <= 0) {
				setState(PURSUIT);
			}
			break;
	}

	avoidTerrain(1.75f, circleCollider.circle.radius + 10);
	// Calculate velocity and update position
	Enemy::update(scene, deltaTime);
}

void SpiderEnemy::attack(Scene& scene) {
	// Create attack point to check for collision against player
	EnemyAttackPoint& attackPoint =
		scene.instantiate<EnemyAttackPoint>(Vec2(position + Vec2(rotation) * 55.0f));
	attackPoint.initializeParent(this);

	setState(EnemyStates::REPOSITION);
}

void SpiderEnemy::attackRangeCheck() {
	Vec2 dist = position - combatScene->player.getPosition();
	constexpr static float atkDist = 70.0f;
	// Enter Attack state when closer than atkDist
	if (dist.dotProduct(dist) <= atkDist * atkDist) {
		setState(EnemyStates::ATTACK);

		// Make spider point directly towards player
		const Vec2 playerDir(combatScene->player.getPosition() - position);
		rotation = playerDir.toDegrees() + 90;
	}
}

void SpiderEnemy::setState(const EnemyStates newState) {
	Enemy::setState(newState);

	switch (newState) {
		using enum EnemyStates;
		case PURSUIT:
			isMoving = true;
			changeAnimation(0);
			break;

		case EVADE:
			isMoving = true;
			changeAnimation(0);
			break;

		case ATTACK:
			isMoving = false;
			animationSpeed = 1;
			changeAnimation(1);
			break;

		case REPOSITION:
			repositionTimer = repositionTime;

			isMoving = true;
			changeAnimation(0);
			break;

		default:
			break;
	}
}

void SpiderEnemy::avoidOtherEnemies(const float strength) {
	try {
		// Move away from closest enemy
		const Enemy* closest = combatScene->getEnemyManager().findClosestEnemy(position);

		const Vec2 dist(position - closest->getPosition());
		constexpr float avoidDist = 150.0f;
		if (dist.dotProduct(dist) <= avoidDist * avoidDist)
			steering += flee(closest->getPosition()) * strength;
	} catch (int e) {
		// Can't find closest enemy. Usually because there is currently only one enemy.
		// Does not require further action, hence why this catch is empty.
	}
}
