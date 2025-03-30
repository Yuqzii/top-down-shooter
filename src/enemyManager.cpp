#include "enemyManager.h"
#include "game/game.h"
#include "enemy.h"

void EnemyManager::update(Game* game, const double& deltaTime) {
	// Remove pointer to enemies that will be deleted
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->deleteObject) {
			it = enemies.erase(it);
		} else it++;
	}

	currentTime -= deltaTime; // Update timer
	// Spawn enemy if timer reaches 0
	if (currentTime <= 0) {
		spawnEnemy(game);
		currentTime = startTime;
	}
}

void EnemyManager::spawnEnemy(Game* game) {
	Enemy* enemy = game->instantiate<Enemy>(vector2Df(200, 200));
	enemies.push_back(enemy);
}
