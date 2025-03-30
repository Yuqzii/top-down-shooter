#include "enemyManager.h"
#include "game/game.h"
#include "enemy.h"

#include <SDL2/SDL_mouse.h>

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

	if (game->mouseInput[SDL_BUTTON_RIGHT]) {
		Tree2D* tree = buildTree();
		//tree->print();

		const vector2Df closestToPlayer = tree->findClosestPosition(game->player->getPivotPosition());
		std::cout << closestToPlayer.x << "	" << closestToPlayer.y << "\n";

		delete tree;
	}
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
