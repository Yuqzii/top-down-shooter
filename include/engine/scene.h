#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "engine/vector2D.h"
#include "engine/Tree2D.h"
#include "enemyManager.h"

class Game;
class Player;

class Scene {
public:
	Scene(Game& gamePtr);
	~Scene() = default;

	/*
	 * @param	persistentObjects	The gameObjects this scene will be initialized with.
	 */
	void initialize(std::vector<std::unique_ptr<GameObject>>& persistentObjects);
	void initialize();

	/*
	* @param	deltaTime	Time between current and last frame in seconds.
	*/
	virtual void update(const float deltaTime);
	virtual void render(SDL_Renderer* renderer);

	// Removes GameObjects marked for deletion. Should be run after updating managers, etc.
	// MUST BE CALLED AT THE END OF EVERY UPDATE.
	void updateDelete();

	/*
	 * @param	position	The position the GameObject is instantiated at.
	 * @return	Pointer to the instantiated GameObject.
	 */
	template<class T>
	T* instantiate(const vector2Df& position) {
		// Compile time check that we don't try to instantiate a non-GameObject
		static_assert(std::is_base_of<GameObject, T>(),
		"Object to instantiate must inherit from GameObject");

		// Create the new GameObject as a unique_ptr to clarify that Scene has ownership
		std::unique_ptr<T> newObject = std::make_unique<T>();
		newObject->initialize(position, *this); // Initialize GameObject
		gameObjects.push_back(std::move(newObject)); // Add GameObject to list

		// Returns the newest GameObject, e.g. the one created now
		return static_cast<T*>(gameObjects.back().get());
	}

	/*
	 * @return	Tree2D of current GameObjects, reference
	 */
	const Tree2D& getObjectTree() const { return objectTree; }

	Game& getGameInstance() const { return game; }

	void reset();

	// TEMPORARY
	const Player* player;
	const EnemyManager& getEnemyManager() const { return enemyManager; }

protected:


private:
	Game& game;

	// TEMPORARY, should be moved to deriving class.
	// Classes inside engine/ should only know about others inside engine/
	EnemyManager enemyManager;

	std::vector<std::unique_ptr<GameObject>> gameObjects; 
	Tree2D objectTree;
	void updateObjectTree();
};
