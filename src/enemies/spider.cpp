#include "enemies/spider.h"
#include "engine/game.h"
#include "scenes/combat_scene.h"
#include "player.h"

SpiderEnemy::SpiderEnemy(const float startHealth, const float damage, const float moveSpeed,
		const float maxSteer, const float steerMult, const float slowingRadius)
		: Enemy(startHealth, damage, moveSpeed, maxSteer, steerMult, slowingRadius) {

	isAnimated = true;
	circleCollider.radius = 50;

	animationEvents.emplace_back(1, 4, [this](Scene& scene) { attack(scene); });
}

void SpiderEnemy::update(Scene& scene, const float deltaTime) {
	steering = vector2Df(); // Reset steering
	// Different movement depending on the current state
	switch (getState()) {
		case EnemyStates::PURSUIT:
			steering += pursuit(combatScene->player, 0.75f);
			steering += seek(combatScene->player.getPivotPosition()) * 0.75f;
			break;
		case EnemyStates::EVADE:
			steering += evade(combatScene->player, 0.4f);
			break;
		case EnemyStates::ATTACK:
			break;
	}

	// Move away from closest enemy if not attacking
	if (getState() != EnemyStates::ATTACK) {
		try {
			// Move away from closest enemy
			const Enemy* closest = combatScene->getEnemyManager().findClosestEnemy(pivotPosition);
			steering += flee(closest->getPivotPosition()) * 0.7f;
		}
		catch (int e) {
			// Can't find closest enemy. Usually because there is currently only one enemy.
			// Does not require further action, hence why this catch is empty.
		}

		vector2Df dist = pivotPosition - combatScene->player.getPivotPosition();
		constexpr static float atkDist = 70.0f;
		// Enter Attack state when closer than atkDist
		if (dist.crossProduct(dist) <= atkDist * atkDist) {
			setState(EnemyStates::ATTACK);

			// Make spider point directly towards player
			const vector2Df playerDir(combatScene->player.getPivotPosition() - pivotPosition);
			rotation = playerDir.toDegrees() + 90;
		}
	}

	// Calculate velocity and update position
	Enemy::update(scene, deltaTime);
}

void SpiderEnemy::attack(Scene& scene) {
	// Create attack point to check for collision against player
	EnemyAttackPoint& attackPoint = scene.instantiate<EnemyAttackPoint>(
			vector2Df(pivotPosition + vector2Df(rotation) * 55.0f));
	attackPoint.initializeParent(this);

	setState(EnemyStates::PURSUIT);
}

void SpiderEnemy::setState(const EnemyStates newState) {
	Enemy::setState(newState);

	switch (newState) {
		case EnemyStates::ATTACK:
			isMoving = false;
			animationSpeed = 1;
			changeAnimation(1);
			break;
		case EnemyStates::PURSUIT:
			isMoving = true;
			changeAnimation(0);
			break;
		case EnemyStates::EVADE:
			isMoving = true;
			changeAnimation(0);
			break;
		default:
			break;
	}
}
