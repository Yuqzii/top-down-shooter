#pragma once

#include "enemyManager.h"
#include "engine/scene.h"
#include "terrain/terrainManager.h"

class Player;

class CombatScene : public Scene {
public:
	CombatScene(Game& game_);

	void initialize(GameObjectVector& persistentObjects) override;
	void initialize() override;

	void update(const float deltaTime) override;
	void render(SDL_Renderer* renderer) const override;

	const EnemyManager& getEnemyManager() const { return enemyManager; }
	const TerrainManager& getTerrainManager() const { return terrainManager; }

private:
	TerrainManager terrainManager;
	EnemyManager enemyManager;

	TerrainManager generateTerrain();
	const Player& spawnPlayer();

public:
	// Player must be initialized after terrainManager
	const Player& player;
};
