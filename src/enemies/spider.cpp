#include "enemies/spider.h"
#include "scenes/combat_scene.h"
#include "player.h"

SpiderEnemy::SpiderEnemy(const float& startHealth, const float& moveSpeed,
		const float& maxSteer, const float& steerMult, const float& slowingRadius)
		: Enemy(startHealth, moveSpeed, maxSteer, steerMult, slowingRadius) {

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
			break;
		case EnemyStates::EVADE:
			steering += evade(combatScene->player, 0.4f);
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

	// Calculate velocity and update position
	Enemy::update(scene, deltaTime);
}
