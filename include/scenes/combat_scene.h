#pragma once

#include "enemyManager.h"
#include "engine/scene.h"
#include "terrain/chunkManager.h"

class Player;

class CombatScene : public Scene {
public:
	CombatScene(Game& game_);

	void update(const float deltaTime) override;
	void render(SDL_Renderer* renderer) const override;

	const EnemyManager& getEnemyManager() const { return enemyManager; }
	const ChunkManager& getChunkManager() const { return chunkManager; }

private:
	ChunkManager chunkManager;
	EnemyManager enemyManager;

	ChunkManager generateTerrain();
	const Player& spawnPlayer();

public:
	// Player must be initialized after chunkManager
	const Player& player;
};
