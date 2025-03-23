#pragma once

#include <vector>

class Enemy;
class Game;

class EnemySpawner {
public:
	void update(Game* game, const double& deltaTime);

private:
	std::vector<Enemy*> enemies;
	constexpr const static float startTime = 3;
	float currentTime = startTime;

	void spawnEnemy(Game* game);
};
