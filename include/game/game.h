#pragma once

#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "game/gameObject.h"
#include "game/vector2D.h"

class GameObject;

class Game {
public:
	Game(const char* title, int width, int height);
	~Game();

	void addGameObject(std::shared_ptr<GameObject> gameObject);
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	SDL_Window* getWindow() { return window; };
	SDL_Renderer* getRenderer() { return renderer; };

	bool input[256]{};

	double deltaTime;
	vector2D mousePos;

	std::vector<std::shared_ptr<GameObject>> gameObjects;

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint64 prevTime;
};
