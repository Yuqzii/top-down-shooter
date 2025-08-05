#pragma once

#include <vector>

#include "enemies/enemy.h"
#include "engine/Tree2D.h"

class Game;

class EnemyManager {
public:
	EnemyManager(std::vector<Vec2>&& spawnPositions);

	void update(Scene& scene, const float deltaTime);

	// Returns a vector of all enemies
	const std::vector<std::reference_wrapper<Enemy>>& getEnemies() const { return enemies; }
	// Returns the position of the closest enemy
	const Enemy* findClosestEnemy(const Vec2& target) const;

private:
	std::vector<std::reference_wrapper<Enemy>> enemies;
	constexpr static float spawnInterval = 3;
	float spawnTimer = 0;

	std::vector<Vec2> spawnPositions;

	void spawnEnemy(Scene& scene);

	std::unique_ptr<Tree2D> enemyTree;
	void updateTree();
};
