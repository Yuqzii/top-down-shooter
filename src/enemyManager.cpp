#include "enemyManager.h"

#include "enemies/spider.h"
#include "engine/game.h"
#include "engine/scene.h"

EnemyManager::EnemyManager() : enemies{}, enemyTree{} {}

void EnemyManager::update() {
	// Remove pointer to enemies that will be deleted
	for (auto it = enemies.begin(); it != enemies.end();) {
		if (it->get().deleteObject) {
			it = enemies.erase(it);
		} else
			it++;
	}

	updateTree();
}

const Enemy* EnemyManager::findClosestEnemy(const Vec2& target) const {
	try {
		// Try to return as const Enemy*
		return static_cast<const Enemy*>(enemyTree.findClosestObject(target));
	} catch (int e) {
		throw e;
		return nullptr;
	}
}

void EnemyManager::addEnemy(Enemy& enemy) { enemies.emplace_back(enemy); }

void EnemyManager::updateTree() {
	std::vector<std::reference_wrapper<GameObject>> objs;
	objs.reserve(enemies.size());
	for (Enemy& enemy : enemies) {
		objs.emplace_back(enemy);
	}
	enemyTree = Tree2D{objs};
}

EnemySpawner::EnemySpawner(EnemyManager& manager) : manager{manager}, timer{timeMax} {}

void EnemySpawner::update(Scene& scene, const float deltaTime) {
	if (spawnPositions.empty()) return;

	timer -= deltaTime;
	if (timer <= 0) {
		spawn(scene);
		std::uniform_real_distribution<float> dist{timeMin, timeMax};
		timer = dist(scene.getGame().randGen);
	}
}

void EnemySpawner::updateSpawnPositions(std::vector<Vec2>&& newSpawns) {
	spawnPositions = std::move(newSpawns);
}

void EnemySpawner::spawn(Scene& scene) {
	std::uniform_int_distribution<std::size_t> dist{0, spawnPositions.size() - 1};
	const std::size_t idx = dist(scene.getGame().randGen);

	Enemy& enemy = scene.instantiate<SpiderEnemy>(spawnPositions[idx]);
	manager.addEnemy(enemy);
}
