#include "scenes/combat_scene.h"

#include <ctime>
#include <iostream>

#include "SDL2/SDL_mouse.h"
#include "terrain/chunk.h"
#include "terrain/terrain.h"
#include "terrain/terrainGenerator.h"

CombatScene::CombatScene(Game& game)
    : Scene{game}, enemyManager{}, chunkManager{generateTerrain()}, player{spawnPlayer()} {}

void CombatScene::update(const float deltaTime) {
	if (getGame().getOnMouseDown()[SDL_BUTTON_RIGHT]) {
		const Vec2 pos = getGame().getMousePos() + cam.getPos();
		chunkManager.changeTerrainInRange(pos, 5, 0);
	}

	Scene::update(deltaTime);

	chunkManager.update(deltaTime, player.getPosition());

	Scene::updateCollision();
	chunkManager.collisionUpdate();

	enemyManager.update();
}

void CombatScene::render(SDL_Renderer* renderer) const {
	Scene::render(renderer);

	chunkManager.render(renderer, getCam());
}

ChunkManager CombatScene::generateTerrain() {
	TerrainGenerator gen{game.randGen};
	// Shape parameters
	gen.shapeFillProb = 0.2;
	gen.shapeGenerations = 5;
	gen.shapeCalcCloseRange = 1;
	gen.shapeCalcFarRange = 4;
	gen.shapeCalcMinCloseFill = 5;
	gen.shapeCalcMaxFarFill = 15;
	gen.shapeWallRandomness = 0.3;
	gen.shapeConsecutiveWallRange = 2;
	gen.shapeMinConsecutiveWall = 2;

	// Corner parameters
	gen.cornerFillProb = 0.3;
	gen.cornerGenerations = 3;
	gen.cornerCalcRange = 1;
	gen.cornerCalcMinFill = 4;

	// Details parameters
	gen.detailsGenerations = 1;
	gen.detailsCalcRange = 2;
	gen.detailsCalcMinFill = 8;

	gen.edgeThickness = 100;

	game.randGen.seed(std::random_device{}());
	constexpr std::size_t terrainXSize = 500;
	constexpr std::size_t terrainYSize = 500;
	constexpr std::size_t shapeSize = 10;
	Terrain terrain = gen.generateTerrain(terrainXSize, terrainYSize, shapeSize);

	constexpr std::size_t chunkSize = 100;
	constexpr int pixelSizeMultiplier = 3;
	constexpr SDL_Color terrainColor{56, 28, 40, 255};
	ChunkManager manager{terrain,      chunkSize, pixelSizeMultiplier,
	                     terrainColor, *this,     enemyManager};
	return manager;
}

const Player& CombatScene::spawnPlayer() {
	const auto spawns = chunkManager.getAllSpawns();
	std::uniform_int_distribution<std::size_t> dist{0, spawns.size() - 1};
	const std::size_t idx = dist(game.randGen);
	const Vec2 spawnPos = spawns[idx];

	const Player& player = instantiate<Player>(spawnPos, &cam);
	std::cout << "Spawned player at " << spawnPos << std::endl;
	return player;
}
