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

const Enemy* EnemyManager::findClosestEnemy(const vector2Df& target) const {
	try {
		// Try to return as const Enemy*
		return static_cast<const Enemy*>(enemyTree->findClosestObject(target));
	}
	catch (int e) {
		throw e;
		return nullptr;
	}
}

void EnemyManager::spawnEnemy(Game* game) {
	Enemy* enemy = game->instantiate<Enemy>(vector2Df(200, 200));
	enemies.push_back(enemy);
}

void EnemyManager::updateTree() {
	enemyTree = std::make_unique<Tree2D>(); // Create new tree
	// Initialize tree with list of enemies
	enemyTree->initializeWithList(std::vector<GameObject*>(enemies.begin(), enemies.end()));
}
