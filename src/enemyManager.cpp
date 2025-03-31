#include <SDL2/SDL_mouse.h>
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

	// Update tree
	updateTree();
}

vector2Df EnemyManager::findClosestEnemy(const vector2Df& target) const {
	vector2Df result;
	try {
		result = enemyTree->findClosestPoint(target);
	}
	catch (int e) {
		throw e;
	}

	return result;
}

void EnemyManager::spawnEnemy(Game* game) {
	Enemy* enemy = game->instantiate<Enemy>(vector2Df(200, 200));
	enemies.push_back(enemy);
}

void EnemyManager::updateTree() {
	// Get positions of all enemies in list
	std::vector<vector2Df> enemyPositions;
	enemyPositions.reserve(enemies.size());

	for (Enemy* enemy : enemies) {
		enemyPositions.push_back(enemy->getPivotPosition());
	}

	enemyTree = std::make_unique<Tree2D>(); // Create new tree
	enemyTree->initializeWithList(enemyPositions); // Build tree with the list
}
