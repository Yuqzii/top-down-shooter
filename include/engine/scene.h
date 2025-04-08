#pragma once

#include <memory>
#include "engine/vector2D.h"
#include "engine/Tree2D.h"

class Game;

class Scene {
public:
	~Scene() = default;

	/*
	 * @param	persistentObjects	The gameObjects this scene will be initialized with.
	 */
	void initialize(std::vector<std::unique_ptr<GameObject>>& persistentObjects);

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
		newObject->initialize(position, this); // Initialize GameObject
		gameObjects.push_back(std::move(newObject)); // Add GameObject to list

		// Returns the newest GameObject, e.g. the one created now
		return static_cast<T*>(gameObjects.back().get());
	}

	/*
	* @return	Tree2D of current GameObjects, reference
	*/
	const Tree2D& getObjectTree() const { return objectTree; }

	void reset();

protected:
	/*
	* @param	game		Pointer to game instance.
	* @param	deltaTime	Time between current and last frame in seconds.
	*/
	virtual void update(const Game* game, const float deltaTime);
	virtual void render(const Game* game);

	// Removes GameObjects marked for deletion. Should be run after updating managers, etc.
	// MUST BE CALLED AT THE END OF EVERY UPDATE.
	void updateDelete();

private:

	std::vector<std::unique_ptr<GameObject>> gameObjects; 
	Tree2D objectTree;
	void updateObjectTree();
};
