#pragma once

#include <list>
#include <memory>
#include "SDL2/SDL.h"
#include "game/UI/UIManager.h"
#include "game/vector2D.h"
#include "game/gameObject.h"
#include "player.h"
#include "enemySpawner.h"

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
	SDL_Window* getWindow() const { return window; };
	SDL_Renderer* getRenderer() const { return renderer; };

	const EnemySpawner getEnemySpawner() const { return enemySpawner; }
	UI::UIManager* getUIManager() { return &uiManager; }

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

	EnemySpawner enemySpawner;
	UI::UIManager uiManager;
};
