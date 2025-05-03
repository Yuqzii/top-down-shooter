#include "engine/scene.h"

#include <ranges>

#include "bullet.h"
#include "enemies/enemy.h"
#include "engine/game.h"
#include "engine/gameObject.h"
#include "player.h"

Scene::Scene(Game& game_) : game(game_) {}

void Scene::initialize(std::vector<std::unique_ptr<GameObject>>& persistentObjects) {
	// Transfer ownership of persistent GameObjects to this scene
	gameObjects = std::move(persistentObjects);
}

void Scene::initialize() {
	// Reserve space for GameObjects to ensure stability
	gameObjects.reserve(1 << 16);
}

void Scene::reset() { gameObjects.clear(); }

void Scene::update(const float deltaTime) {
	// Update all GameObjects
	for (auto& object : gameObjects) {
		object->update(*this, deltaTime);
	}

	updateObjectTree();

	// Check for collisions after all GameObjects are updated
	for (auto& object : gameObjects) {
		if (object->getCollider() == nullptr) continue;		 // Collider does not exist
		if (object->getCollider()->getIsStatic()) continue;	 // Don't check static colliders
		object->getCollider()->checkCollisions(*this);
	}

	// Update GameObjects according to registered collisions
	for (auto& object : gameObjects) {
		if (object->getCollider() == nullptr) continue;
		object->getCollider()->collisionUpdate(*this);
	}
}

void Scene::updateDelete() {
	// Delete objects marked for deletion
	for (auto it = gameObjects.begin(); it != gameObjects.end();) {
		if (it->get()->deleteObject) {
			it = gameObjects.erase(it);	 // Delete GameObject
		} else
			it++;
	}
}

void Scene::render(SDL_Renderer* renderer) const {
	for (auto& object : gameObjects) {
		object->render(renderer);
	}
}

void Scene::updateObjectTree() {
	objectTree = Tree2D();	// Create new tree

	// Create vector of raw pointers from unique_ptr vector
	const auto objectsRange =
		gameObjects |
		std::views::transform(
			[](const std::unique_ptr<GameObject>& ptr) -> GameObject* { return ptr.get(); });
	const std::vector<GameObject*> rawObjects(objectsRange.begin(), objectsRange.end());

	// Create the tree from the GameObject vector
	objectTree.initializeWithList(rawObjects);
}
