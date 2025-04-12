#pragma once

#include <memory>
#include <vector>

#include "SDL2/SDL.h"
#include "engine/gameObject.h"
#include "engine/renderManager.h"
#include "engine/vector2D.h"

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

	template <class T>
	void addScene();

	// SDL stuff
	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	RenderManager& getRenderManager() { return renderManager; }

	const std::array<bool, 256>& getInput() const { return input; }
	const std::array<bool, 32>& getMouseInput() const { return mouseInput; }
	const vector2Df& getMousePos() const { return mousePos; }

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::array<bool, 256> input;
	std::array<bool, 32> mouseInput;
	vector2Df mousePos;

	Uint64 prevTime;

	std::vector<std::unique_ptr<Scene>> scenes;
	uint16_t currentScene;
	constexpr static const uint16_t sceneCount = 1;

	RenderManager renderManager;
};
