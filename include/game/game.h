#pragma once

#include <list>
#include <memory>
#include "SDL2/SDL.h"
#include "game/vector2D.h"
#include "game/gameObject.h"
#include "player.h"

class GameObject;
class Player;

class Game {
public:
	Game(const char* title, int width, int height);
	~Game();

	// Function to instantiate GameObjects
	// Returns a raw pointer to the instantiated object
	template<class T>
	T* instantiate(const std::string textureSheet, const vector2Df position);
	
	// Game loop
	void handleEvents();
	void update();
	void render() const;
	void clean();

	bool running() { return isRunning; };

	// SDL stuff
	SDL_Window* getWindow() { return window; };
	SDL_Renderer* getRenderer() { return renderer; };

	bool input[256]{};
	bool mouseInput[32]{};

	double deltaTime;
	vector2D mousePos;

	const Player* player;

	std::list<std::unique_ptr<GameObject>> gameObjects; 

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint64 prevTime;
};
