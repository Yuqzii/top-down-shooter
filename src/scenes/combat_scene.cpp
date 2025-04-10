#include "scenes/combat_scene.h"

void CombatScene::initialize() {
	Scene::initialize();

	player = instantiate<Player>(vector2Df(500, 750));
	enemyManager = EnemyManager();
}

void CombatScene::initialize(GameObjectVector& persistentObjects) {
	Scene::initialize(persistentObjects);

	player = instantiate<Player>(vector2Df(500, 750));
	enemyManager = EnemyManager();
}

void CombatScene::update(const float deltaTime) {
	Scene::update(deltaTime);

	enemyManager.update(*this, deltaTime);
}
