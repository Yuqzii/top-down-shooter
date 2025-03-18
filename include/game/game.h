#pragma once

#include "SDL2/SDL.h"

class Game {

public:
	Game(const char* title, int width, int height);
	~Game();
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
};
