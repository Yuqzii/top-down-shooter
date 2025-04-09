#pragma once

#include <list>
#include <memory>
#include "SDL2/SDL.h"
#include "engine/renderManager.h"
#include "engine/vector2D.h"
#include "engine/gameObject.h"

class Scene;

class Game {
public:
	Game(const char* title, const int width, const int height);
	~Game();
	
	// Game loop
	void handleEvents();
	void update();
	void render() const;
	void clean();

	bool running() const { return isRunning; };

	void changeScene(const int sceneIndex);

	// SDL stuff
	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	RenderManager& getRenderManager() { return renderManager; }

	bool input[256]{};
	bool mouseInput[32]{};

	vector2D mousePos;

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint64 prevTime;

	std::vector<std::unique_ptr<Scene>> scenes;
	unsigned int currentScene;

	RenderManager renderManager;
};
