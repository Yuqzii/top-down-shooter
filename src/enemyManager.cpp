#include "enemyManager.h"

#include "enemies/spider.h"
#include "engine/game.h"
#include "engine/scene.h"

EnemyManager::EnemyManager(std::vector<Vec2>&& spawnPositions) : spawnPositions{spawnPositions} {}

void EnemyManager::update(Scene& scene, const float deltaTime) {
	// Remove pointer to enemies that will be deleted
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) {
		if ((*it)->deleteObject) {
			it = enemies.erase(it);
		} else
			it++;
	}

	spawnTimer -= deltaTime;  // Update timer
	// Spawn enemy if timer reaches 0
	if (spawnTimer <= 0) {
		spawnEnemy(scene);
		spawnTimer = spawnInterval;
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
	std::uniform_int_distribution<std::size_t> dist{0, spawnPositions.size() - 1};
	const std::size_t idx = dist(scene.getGame().randGen);

	Enemy& enemy = scene.instantiate<SpiderEnemy>(spawnPositions[idx]);
	enemies.push_back(&enemy);
}

void EnemyManager::updateTree() {
	enemyTree = std::make_unique<Tree2D>();
	enemyTree->initializeWithList(std::vector<GameObject*>(enemies.begin(), enemies.end()));
}
