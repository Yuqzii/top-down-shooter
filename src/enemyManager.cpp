#include "enemyManager.h"

#include "enemies/spider.h"
#include "engine/game.h"
#include "engine/scene.h"

const std::array<const Vec2, 4> spawnPositions = {Vec2{200, 170}, Vec2{200, 550},
															Vec2{1100, 350}, Vec2{1200, 740}};

void EnemyManager::update(Scene& scene, const float deltaTime) {
	// Remove pointer to enemies that will be deleted
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->deleteObject) {
			it = enemies.erase(it);
		} else
			it++;
	}

	currentTime -= deltaTime;  // Update timer
	// Spawn enemy if timer reaches 0
	if (currentTime <= 0) {
		spawnEnemy(scene);
		currentTime = startTime;
	}

	// Update tree
	updateTree();
}

const Enemy* EnemyManager::findClosestEnemy(const Vec2& target) const {
	try {
		// Try to return as const Enemy*
		return static_cast<const Enemy*>(enemyTree->findClosestObject(target));
	} catch (int e) {
		throw e;
		return nullptr;
	}
}

void EnemyManager::spawnEnemy(Scene& scene) {
	srand(time(NULL));
	int randIdx = rand() % spawnPositions.size();

	Enemy& enemy = scene.instantiate<SpiderEnemy>(spawnPositions[randIdx]);
	enemies.push_back(&enemy);
}

void EnemyManager::updateTree() {
	enemyTree = std::make_unique<Tree2D>();	 // Create new tree
	// Initialize tree with list of enemies
	enemyTree->initializeWithList(std::vector<GameObject*>(enemies.begin(), enemies.end()));
}
