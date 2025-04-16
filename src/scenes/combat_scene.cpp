#include "scenes/combat_scene.h"

void CombatScene::initialize() {
	Scene::initialize();

	enemyManager = EnemyManager();
}

void CombatScene::initialize(GameObjectVector& persistentObjects) {
	Scene::initialize(persistentObjects);

	enemyManager = EnemyManager();
}

void CombatScene::update(const float deltaTime) {
	Scene::update(deltaTime);

	enemyManager.update(*this, deltaTime);
}

void CombatScene::render(SDL_Renderer* renderer) const {
	Scene::render(renderer);

	terrainManager.render(renderer);
}
