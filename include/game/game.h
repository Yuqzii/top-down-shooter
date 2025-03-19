#pragma once

#include "SDL2/SDL.h"
#include "game/gameObject.h"
#include <memory>
#include <vector>

class Game {

public:
	Game(const char* title, int width, int height);
	~Game();
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	double deltaTime;

	std::vector<std::unique_ptr<GameObject>> gameObjects;

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;

	Uint64 prevTime;
};
