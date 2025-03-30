#pragma once

#include <vector>
#include "game/2DTree.h"
#include "enemy.h"

class Game;

class EnemyManager {
public:
	void update(Game* game, const double& deltaTime);

	const std::vector<Enemy*> getEnemies() const { return enemies; }
	const Enemy* findClosestEnemy(const vector2Df& target) const;

private:
	std::vector<Enemy*> enemies;
	constexpr const static float startTime = 1;
	float currentTime = startTime;

	void spawnEnemy(Game* game);

	std::unique_ptr<Tree2D> enemyTree;
	Tree2D* buildTree();
};
