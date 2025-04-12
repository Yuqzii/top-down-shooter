#pragma once

#include <vector>

#include "enemies/enemy.h"
#include "engine/Tree2D.h"

class Game;

class EnemyManager {
public:
	void update(Scene& scene, const float deltaTime);

	// Returns a vector of all enemies
	const std::vector<Enemy*> getEnemies() const { return enemies; }
	// Returns the position of the closest enemy
	const Enemy* findClosestEnemy(const vector2Df& target) const;

private:
	std::vector<Enemy*> enemies;
	constexpr const static float startTime = 3;
	float currentTime = 0;

	void spawnEnemy(Scene& scene);

	std::unique_ptr<Tree2D> enemyTree;
	void updateTree();
};
