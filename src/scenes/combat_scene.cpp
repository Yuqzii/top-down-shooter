#include "scenes/combat_scene.h"

#include <ctime>

#include "SDL2/SDL_mouse.h"
#include "scenes/combat_map.h"
#include "terrain/chunk.h"
#include "terrain/terrain.h"
#include "terrain/terrainGenerator.h"

CombatScene::CombatScene(Game& game_)
	: Scene{game_}, player{instantiate<Player>(Vec2{700, 400}, &cam)} {
	TerrainGenerator gen;
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

	gen.seed = std::time({});
	Terrain terrain = gen.generateTerrain(500, 500, 10);

	constexpr std::size_t chunkSize = 100;
	constexpr int pixelSizeMultiplier = 3;
	terrainManager = std::make_unique<TerrainManager>(terrain, chunkSize, pixelSizeMultiplier,
													  SDL_Color{56, 28, 40, 255}, *this);
}

void CombatScene::initialize() {
	Scene::initialize();

	enemyManager = EnemyManager();
	terrainManager->updateColliders();
	terrainManager->updateRender();
}

void CombatScene::initialize(GameObjectVector& persistentObjects) {
	Scene::initialize(persistentObjects);

	enemyManager = EnemyManager();
}

void CombatScene::update(const float deltaTime) {
	if (getGame().getOnMouseDown()[SDL_BUTTON_RIGHT]) {
		const Vec2 pos = getGame().getMousePos() + cam.getPos();
		terrainManager->setCellsInRange(pos, 5, 0);
	}

	Scene::update(deltaTime);

	enemyManager.update(*this, deltaTime);
}

void CombatScene::render(SDL_Renderer* renderer) const {
	Scene::render(renderer);

	terrainManager->render(renderer, getCam());
}
