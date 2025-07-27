#include "scenes/combat_scene.h"

#include <ctime>

#include "SDL2/SDL_mouse.h"
#include "scenes/combat_map.h"
#include "terrain/terrain.h"
#include "terrain/terrainGenerator.h"

CombatScene::CombatScene(Game& game_)
	: Scene{game_}, player{instantiate<Player>(Vec2{700, 400}, &cam)} {
	TerrainGenerator gen;
	gen.setShapeGenerations(5);
	gen.setShapeFillProb(0.2);
	gen.setShapeWallRandomness(0.4);
	gen.setShapeConsecutiveWallRange(1);
	gen.setShapeMinConsecutiveWall(3);
	gen.setCornerFillProb(0.3);
	gen.setCornerGenerations(3);
	gen.setDetailsGenerations(2);

	gen.setSeed(std::time({}));
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
