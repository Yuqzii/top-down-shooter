#pragma once

#include "enemyManager.h"
#include "terrain/terrainManager.h"
#include "engine/scene.h"

class Player;

class CombatScene : public Scene {
public:
	CombatScene(Game& game_);

	void initialize(GameObjectVector& persistentObjects) override;
	void initialize() override;

	void update(const float deltaTime) override;
	void render(SDL_Renderer* renderer) const override;

	const Player& player;
	const EnemyManager& getEnemyManager() const { return enemyManager; }

private:
	EnemyManager enemyManager;
	TerrainManager terrainManager;
};
