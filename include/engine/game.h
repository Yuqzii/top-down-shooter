#pragma once

#include <list>
#include <memory>
#include "SDL2/SDL.h"
#include "engine/renderManager.h"
#include "engine/vector2D.h"
#include "engine/gameObject.h"
#include "player.h"
#include "enemyManager.h"

class GameObject;
class Player;

class Game {
public:
	Game(const char* title, int width, int height);
	~Game();

	// Function to instantiate GameObjects
	template<class T>
	T* instantiate(const vector2Df& position);
	
	// Game loop
	void handleEvents();
	void update();
	void render() const;
	void clean();

	bool running() { return isRunning; };

	// SDL stuff
	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	const EnemyManager* getEnemyManager() const { return &enemyManager; }
	RenderManager* getRenderManager() { return &renderManager; }
	const Tree2D& getObjectTree() const { return objectTree; }

	bool input[256]{};
	bool mouseInput[32]{};

	double deltaTime;
	vector2D mousePos;

	const Player* player;

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint64 prevTime;

	std::vector<std::unique_ptr<GameObject>> gameObjects; 
	Tree2D objectTree;
	void updateObjectTree();

	EnemyManager enemyManager;
	RenderManager renderManager;
};
