#pragma once

#include <vector>

#include "enemies/enemy.h"
#include "engine/Tree2D.h"

class Game;

class EnemyManager {
public:
	EnemyManager();

	void update();

	void addEnemy(Enemy& enemy);

	// Returns a vector of all enemies
	const std::vector<std::reference_wrapper<Enemy>>& getEnemies() const { return enemies; }
	// Returns the position of the closest enemy
	const Enemy* findClosestEnemy(const Vec2& target) const;

private:
	std::vector<std::reference_wrapper<Enemy>> enemies;

	Tree2D enemyTree;
	void updateTree();
};

class EnemySpawner {
public:
	EnemySpawner(EnemyManager& manager);

	void update(Scene& scene, const float deltaTime);

	void updateSpawnPositions(std::vector<Vec2>&& newSpawns);

private:
	EnemyManager& manager;

	std::vector<Vec2> spawnPositions;

	float timer;
	static constexpr float timeMin = 2;
	static constexpr float timeMax = 5;

	// Randomly chooses a position from spawnPositions and spawns an enemy there.
	void spawn(Scene& scene);
};
