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
	T* instantiate(const vector2Df& position);

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
