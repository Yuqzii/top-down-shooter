#pragma once

#include <memory>

#include "SDL2/SDL_render.h"
#include "bullet.h"
#include "engine/Tree2D.h"
#include "engine/camera.h"
#include "engine/gameObject.h"
#include "player.h"

class Game;

typedef std::vector<std::unique_ptr<GameObject>> GameObjectVector;

class Scene {
public:
	Scene(Game& game);
	virtual ~Scene() = default;

	/*
	 * @param	persistentObjects	The gameObjects this scene will be initialized with.
	 */
	virtual void initialize(GameObjectVector& persistentObjects);
	virtual void initialize();

	/*
	 * @param	deltaTime	Time between current and last frame in seconds.
	 */
	virtual void update(const float deltaTime);
	virtual void render(SDL_Renderer* renderer) const;

	// Removes GameObjects marked for deletion. Should be run after updating managers, etc.
	// MUST BE CALLED AT THE END OF EVERY UPDATE.
	void updateDelete();

	/*
	 * @param	position	The position the GameObject is instantiated at.
	 * @return	Pointer to the instantiated GameObject.
	 */
	template <class T, typename... Args>
	T& instantiate(Args&&... args) {
		// Compile time check that we don't try to instantiate a non-GameObject
		static_assert(std::is_base_of<GameObject, T>(),
		              "Object to instantiate must inherit from GameObject");

		// Create the new GameObject as a unique_ptr to clarify that Scene has ownership
		std::unique_ptr<T> newObject = std::make_unique<T>();
		newObject->initialize(*this, std::forward<Args>(args)...);
		gameObjects.push_back(std::move(newObject));  // Add GameObject to list

		// Returns the newest GameObject, e.g. the one created now
		return static_cast<T&>(*gameObjects.back().get());
	}

	/*
	 * @return	Tree2D of current GameObjects, reference
	 */
	const Tree2D& getObjectTree() const { return objectTree; }
	const GameObjectVector& getGameObjects() const { return gameObjects; }

	Game& getGame() const { return game; }
	const Camera& getCam() const { return cam; }

	void reset();

protected:
	Camera cam;
	Game& game;

private:
	GameObjectVector gameObjects;
	Tree2D objectTree;
	void updateObjectTree();
};
