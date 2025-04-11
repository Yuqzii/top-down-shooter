#include <iostream>
#include "enemies/spider.h"
#include "engine/game.h"
#include "scenes/combat_scene.h"
#include "player.h"

SpiderEnemy::SpiderEnemy(const float startHealth, const float damage, const float moveSpeed,
		const float maxSteer, const float steerMult, const float slowingRadius)
		: Enemy(startHealth, damage, moveSpeed, maxSteer, steerMult, slowingRadius) {

	isAnimated = true;
	circleCollider.radius = 50;
}

void SpiderEnemy::update(Scene& scene, const float deltaTime) {
	steering = vector2Df(); // Reset steering
	// Different movement depending on the current state
	switch (state) {
		case EnemyStates::PURSUIT:
			steering += pursuit(combatScene->player, 0.75f);
			steering += seek(combatScene->player.getPivotPosition()) * 0.75f;
			changeAnimation(0);
			isMoving = true;
			break;
		case EnemyStates::EVADE:
			steering += evade(combatScene->player, 0.4f);
			changeAnimation(0);
			isMoving = true;
			break;
		case EnemyStates::ATTACK:
			changeAnimation(1);
			animationSpeed = 1;
			isMoving = false;
			break;
	}

	try {
		// Move away from closest enemy
		const Enemy* closest = combatScene->getEnemyManager().findClosestEnemy(pivotPosition);
		steering += flee(closest->getPivotPosition()) * 0.7f;
	}
	catch (int e) {
		// Can't find closest enemy. Usually because there is currently only one enemy.
		// Does not require further action, hence why this catch is empty.
	}

	const vector2Df dist = pivotPosition - combatScene->player.getPivotPosition();
	if (dist.crossProduct(dist) < 7000) {
		state = EnemyStates::ATTACK;
	}

	// Calculate velocity and update position
	Enemy::update(scene, deltaTime);
}

void SpiderEnemy::attack(Scene& scene) {
	EnemyCollisionPoint& attackPoint = scene.instantiate<EnemyCollisionPoint>(
			vector2Df(getPivotPosition() + vector2Df(rotation) * 55.0f));
	attackPoint.initializeParent(this);

	state = EnemyStates::PURSUIT;
}
