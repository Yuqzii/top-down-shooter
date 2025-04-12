#pragma once

#include "enemies/enemy.h"
#include "engine/Tree2D.h"
#include <vector>

class Game;

class EnemyManager {
public:
<<<<<<< HEAD
	void update(Scene& scene, const float deltaTime);
=======
  void update(Scene &scene, const float deltaTime);
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  // Returns a vector of all enemies
  const std::vector<Enemy *> getEnemies() const { return enemies; }
  // Returns the position of the closest enemy
  const Enemy *findClosestEnemy(const vector2Df &target) const;

private:
<<<<<<< HEAD
	std::vector<Enemy*> enemies;
	constexpr const static float startTime = 3;
	float currentTime = 0;
=======
  std::vector<Enemy *> enemies;
  constexpr const static float startTime = 3;
  float currentTime = 0;
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void spawnEnemy(Scene &scene);

  std::unique_ptr<Tree2D> enemyTree;
  void updateTree();
};
