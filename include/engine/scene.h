#pragma once

#include <memory>

#include "SDL2/SDL_render.h"
#include "engine/Tree2D.h"
#include "engine/vector2D.h"

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
	template <class T>
	T& instantiate(const vector2Df& position);

	/*
	 * @return	Tree2D of current GameObjects, reference
	 */
	const Tree2D& getObjectTree() const { return objectTree; }

	Game& getGame() const { return game; }

	void reset();

private:
	Game& game;

	GameObjectVector gameObjects;
	Tree2D objectTree;
	void updateObjectTree();
};
