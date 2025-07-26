#include "scenes/combat_scene.h"

#include <ctime>

#include "SDL2/SDL_mouse.h"
#include "scenes/combat_map.h"
#include "terrain/terrain.h"
#include "terrain/terrainGenerator.h"

CombatScene::CombatScene(Game& game_)
	: Scene{game_}, player{instantiate<Player>(Vec2{700, 400}, &cam)} {
	const unsigned int mapSeed = std::time({});
	constexpr int shapeGenerations = 5;
	constexpr double shapeFillProb = 0.2;
	constexpr int detailsGenerations = 3;
	constexpr double detailsFillProb = 0.4;
	TerrainGenerator gen{mapSeed, shapeGenerations, shapeFillProb, detailsGenerations,
						 detailsFillProb};
	Terrain terrain = gen.generateTerrain(500, 500, 10);

	terrainManager = std::make_unique<TerrainManager>(terrain, SDL_Color{56, 28, 40, 255}, *this);
}

void CombatScene::initialize() {
	Scene::initialize();

	enemyManager = EnemyManager();
	terrainManager->updateCollisions();
	terrainManager->updateRender();
}

void CombatScene::initialize(GameObjectVector& persistentObjects) {
	Scene::initialize(persistentObjects);

	enemyManager = EnemyManager();
}

void CombatScene::update(const float deltaTime) {
	if (getGame().getOnMouseDown()[SDL_BUTTON_RIGHT]) {
		const Vec2 pos = getGame().getMousePos() + cam.getPos();
		terrainManager->removeInRange(pos, 5);
	}

	Scene::update(deltaTime);

	enemyManager.update(*this, deltaTime);
}

void CombatScene::render(SDL_Renderer* renderer) const {
	Scene::render(renderer);

	terrainManager->render(renderer, getCam());
}
