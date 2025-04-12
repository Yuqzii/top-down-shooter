#pragma once

#include "enemyManager.h"
#include "engine/scene.h"

class Player;

class CombatScene : public Scene {
   public:
	CombatScene(Game& game_) : Scene(game_), player(instantiate<Player>(vector2Df(700, 400))) {}

	void initialize(GameObjectVector& persistentObjects) override;
	void initialize() override;

	void update(const float deltaTime) override;

	const Player& player;
	const EnemyManager& getEnemyManager() const { return enemyManager; }

   private:
	EnemyManager enemyManager;
};
