#pragma once

#include <array>
#include <memory>
#include <vector>

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
	constexpr static const int pixelSize = 3;

	RenderManager& getRenderManager() { return renderManager; }

	const std::array<bool, 256>& getInput() const { return input; }
	// Button value true if button is being held
	const std::array<bool, 32>& getMouseInput() const { return mouseInput; }
	// Button value true if pressed this frame
	const std::array<bool, 32>& getOnMouseDown() const { return onMouseDown; }
	const vector2Df& getMousePos() const { return mousePos; }

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::array<bool, 256> input;
	std::array<bool, 32> mouseInput;
	std::array<bool, 32> onMouseDown;
	vector2Df mousePos;

	Uint64 prevTime;

	std::vector<std::unique_ptr<Scene>> scenes;
	uint16_t currentScene;
	constexpr static const uint16_t sceneCount = 1;

	RenderManager renderManager;
};
