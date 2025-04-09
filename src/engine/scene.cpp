#include <ranges>
#include "engine/scene.h"
#include "engine/gameObject.h"

Scene::Scene(Game& gamePtr) : game(gamePtr) {}

void Scene::initialize(std::vector<std::unique_ptr<GameObject>>& persistentObjects) {
	// Transfer ownership of persistent GameObjects to this scene
	gameObjects = std::move(persistentObjects);
}

void Scene::initialize() {
	// Reserve space for GameObjects to ensure stability
	gameObjects.reserve(1 << 16);
}

void Scene::reset() {
	gameObjects.clear();
}

void Scene::update(const float deltaTime) {
	// Update all GameObjects
	for (auto& object : gameObjects) {
		object->update(*this, deltaTime);
	}
	
	// Check for collisions after all GameObjects are updated
	for (auto& object : gameObjects) {
		object->checkCollisions(*this);
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

void Scene::render(SDL_Renderer* renderer) {
	for (auto& object : gameObjects) {
		object->render(renderer);
	}
}

void Scene::updateObjectTree() {
	objectTree = Tree2D(); // Create new tree

	// Create vector of raw pointers from unique_ptr vector
	const auto objectsRange = gameObjects
			| std::views::transform(
			[](const std::unique_ptr<GameObject>& ptr) -> GameObject* { return ptr.get(); });
	const std::vector<GameObject*> rawObjects(objectsRange.begin(), objectsRange.end());

	// Create the tree from the GameObject vector
	objectTree.initializeWithList(rawObjects);
}
