#include "engine/scene.h"

#include <ranges>

#include "bullet.h"
#include "enemies/enemy.h"
#include "enemies/spider.h"
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
		if (object->getCollider() == nullptr) continue;
		object->getCollider()->checkCollisions(*this);
	}

	// Update GameObjects according to registered collisions
	for (auto& object : gameObjects) {
		if (object->getCollider() == nullptr) continue;
		object->getCollider()->collisionUpdate();
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

template <class T>
T& Scene::instantiate(const vector2Df& position) {
	// Compile time check that we don't try to instantiate a non-GameObject
	static_assert(std::is_base_of<GameObject, T>(),
				  "Object to instantiate must inherit from GameObject");

	// Create the new GameObject as a unique_ptr to clarify that Scene has ownership
	std::unique_ptr<T> newObject = std::make_unique<T>();
	newObject->initialize(position, *this);		  // Initialize GameObject
	gameObjects.push_back(std::move(newObject));  // Add GameObject to list

	// Returns the newest GameObject, e.g. the one created now
	return static_cast<T&>(*gameObjects.back().get());
}
// Create all valid templates
template GameObject& Scene::instantiate<GameObject>(const vector2Df& position);
template Player& Scene::instantiate<Player>(const vector2Df& position);
template Bullet& Scene::instantiate<Bullet>(const vector2Df& position);
template SpiderEnemy& Scene::instantiate<SpiderEnemy>(const vector2Df& position);
template EnemyAttackPoint& Scene::instantiate<EnemyAttackPoint>(const vector2Df& position);

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
