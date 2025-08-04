#include "scenes/combat_scene.h"

#include <ctime>
#include <iostream>

#include "SDL2/SDL_mouse.h"
#include "terrain/chunk.h"
#include "terrain/terrain.h"
#include "terrain/terrainGenerator.h"

CombatScene::CombatScene(Game& game)
	: Scene{game},
	  terrainManager{generateTerrain()},
	  player{spawnPlayer()},
	  enemyManager{std::move(terrainManager.getAllSpawns())} {}

void CombatScene::initialize() {
	Scene::initialize();

	terrainManager.updateColliders();
	terrainManager.updateRender();
}

void CombatScene::initialize(GameObjectVector& persistentObjects) {
	Scene::initialize(persistentObjects);
}

void CombatScene::update(const float deltaTime) {
	if (getGame().getOnMouseDown()[SDL_BUTTON_RIGHT]) {
		const Vec2 pos = getGame().getMousePos() + cam.getPos();
		terrainManager.setCellsInRange(pos, 5, 0);
	}

	Scene::update(deltaTime);

	enemyManager.update(*this, deltaTime);
}

void CombatScene::render(SDL_Renderer* renderer) const {
	Scene::render(renderer);

	terrainManager.render(renderer, getCam());
}

TerrainManager CombatScene::generateTerrain() {
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
	Terrain terrain = gen.generateTerrain(500, 500, 10);

	constexpr std::size_t chunkSize = 100;
	constexpr int pixelSizeMultiplier = 3;
	constexpr SDL_Color terrainColor{56, 28, 40, 255};
	TerrainManager manager{terrain, chunkSize, pixelSizeMultiplier, terrainColor, *this};
	return manager;
}

const Player& CombatScene::spawnPlayer() {
	const auto spawns = terrainManager.getAllSpawns();
	std::uniform_int_distribution<std::size_t> dist{0, spawns.size() - 1};
	const std::size_t idx = dist(game.randGen);
	const Vec2 spawnPos = spawns[idx];

	std::cout << "Spawning player at " << spawnPos << std::endl;
	const Player& player = instantiate<Player>(spawnPos, &cam);
	return player;
}
