#pragma once

#include "engine/scene.h"
#include "enemyManager.h"

class Player;

class CombatScene : public Scene {
public:
<<<<<<< HEAD
<<<<<<< HEAD
	CombatScene(Game& game_) : Scene(game_), player(instantiate<Player>(vector2Df(700, 400))) {}
=======
  CombatScene(Game &game_)
      : Scene(game_), player(instantiate<Player>(vector2Df(700, 400))) {}
>>>>>>> parent of 3336e98 (chore: fix linting issues)
=======
	CombatScene(Game& game_) : Scene(game_), player(instantiate<Player>(vector2Df(700, 400))) {}
>>>>>>> parent of 208a3b6 (chore: fix linting issues)

	void initialize(GameObjectVector& persistentObjects) override;
	void initialize() override;

	void update(const float deltaTime) override;

	const Player& player;
	const EnemyManager& getEnemyManager() const { return enemyManager; }

private:
<<<<<<< HEAD
<<<<<<< HEAD
	EnemyManager enemyManager;
=======
  EnemyManager enemyManager;
>>>>>>> parent of 3336e98 (chore: fix linting issues)
=======
	EnemyManager enemyManager;

>>>>>>> parent of 208a3b6 (chore: fix linting issues)
};
