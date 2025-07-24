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

	const Player& player;
	const EnemyManager& getEnemyManager() const { return enemyManager; }
	const TerrainManager& getTerrainManager() const { return *terrainManager.get(); }

private:
	EnemyManager enemyManager;
	std::unique_ptr<TerrainManager> terrainManager;
};
