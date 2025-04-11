#include "engine/scene.h"
#include "enemyManager.h"
#include "engine/game.h"
#include "enemies/enemy.h"

void EnemyManager::update(Scene& scene, const float deltaTime) {
	// Remove pointer to enemies that will be deleted
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->deleteObject) {
			it = enemies.erase(it);
		} else it++;
	}

	currentTime -= deltaTime; // Update timer
	// Spawn enemy if timer reaches 0
	if (currentTime <= 0) {
		spawnEnemy(scene);
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

void EnemyManager::spawnEnemy(Scene& scene) {
	Enemy& enemy = scene.instantiate<Enemy>(vector2Df(200, 200));
	enemies.push_back(&enemy);
}

void EnemyManager::updateTree() {
	enemyTree = std::make_unique<Tree2D>(); // Create new tree
	// Initialize tree with list of enemies
	enemyTree->initializeWithList(std::vector<GameObject*>(enemies.begin(), enemies.end()));
}
