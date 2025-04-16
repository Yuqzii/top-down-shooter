#pragma once

#include "enemyManager.h"
#include "terrain/terrainManager.h"
#include "engine/scene.h"

class Player;

const std::vector<std::vector<char>> terrainMap {
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0},
	std::vector<char>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	std::vector<char>{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	std::vector<char>{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
};

class CombatScene : public Scene {
public:
	CombatScene(Game& game_) :
		Scene{game_},
		player{instantiate<Player>(vector2Df(700, 400))},
		terrainManager{terrainMap}
	{}

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
