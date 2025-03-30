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
	enemyTree = std::unique_ptr<Tree2D>(buildTree());
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

Tree2D* EnemyManager::buildTree() {
	Tree2D* tree = new Tree2D();

	// Add every enemy to the tree
	for (Enemy* enemy : enemies) {
		tree->insert(enemy->getPivotPosition());
	}

	return tree;
}
