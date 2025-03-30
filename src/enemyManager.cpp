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

	if (game->mouseInput[SDL_BUTTON_RIGHT]) {
		//tree->print();

		for (Enemy* enemy : enemies) {
			vector2Df closest;
			try {
				closest = enemyTree->findClosestPosition(enemy->getPivotPosition());
			}
			catch (int e) {
				std::cerr << "Exception occurred. Could not find closest enemy. Error code: "
					<< e << std::endl;
				continue;
			}
			std::cout << "The closest to enemy at position: (" << enemy->getPivotPosition().x <<
				", " << enemy->getPivotPosition().y << ") is (" << closest.x << ", " << closest.y
				<< ")\n";
		}
		std::cout << std::endl;
	}
}

const Enemy* EnemyManager::findClosestEnemy(const vector2Df& target) const {
	const Enemy* result = nullptr;
	try {
		result = enemyTree->findClosestEnemy(target);
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
		tree->insert(enemy);
	}

	return tree;
}
