#pragma once

#include <vector>
#include "game/Tree2D.h"
#include "enemy.h"

class Game;

class EnemyManager {
public:
	void update(Game* game, const double& deltaTime);

	// Returns a vector of all enemies
	const std::vector<Enemy*> getEnemies() const { return enemies; }
	// Returns the position of the closest enemy
	const Enemy* findClosestEnemy(const vector2Df& target) const;

private:
	std::vector<Enemy*> enemies;
	constexpr const static float startTime = 1;
	float currentTime = startTime;

	void spawnEnemy(Game* game);

	std::unique_ptr<Tree2D> enemyTree;
	void updateTree();
};
