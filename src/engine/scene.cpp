#include "engine/scene.h"
#include "engine/gameObject.h"

void Scene::initialize(std::vector<std::unique_ptr<GameObject>>& persistentObjects) {
	// Transfer ownership of persistent GameObjects to this scene
	gameObjects = std::move(persistentObjects);
}

void Scene::reset() {
	gameObjects.clear();
}

void Scene::update(const Game* game, const float deltaTime) {
	// Update all GameObjects
	for (auto& object : gameObjects) {
		object->update(game, deltaTime);
	}
	
	// Check for collisions after all GameObjects are updated
	for (auto& object : gameObjects) {
		object->checkCollisions(game);
	}

	// Update GameObjects according to registered collisions
	for (auto& object : gameObjects) {
		object->collisionUpdate();
	}
}

void Scene::updateDelete() {
	// Delete objects marked for deletion
	for (auto it = gameObjects.begin(); it != gameObjects.end();) {
		if (it->get()->deleteObject) {
			it = gameObjects.erase(it); // Delete GameObject
		}
		else it++;
	}
}

void Scene::render(const Game* game) {

}
