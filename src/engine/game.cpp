#include <iostream>
#include "SDL2/SDL_image.h"
#include "engine/game.h"
#include "engine/scene.h"

Game::Game(const char* title, const int width, const int height) {
	isRunning = true;

	// Check that SDL initializes
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not be initialized:\n" << SDL_GetError();
		isRunning = false;
		return;
	}

	// Create window and renderer
	window = nullptr;
	renderer = nullptr;
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	SDL_SetWindowTitle(window, title); // Set title

	int flagsIMG = IMG_INIT_PNG;
	if ((IMG_Init(flagsIMG) & flagsIMG) != flagsIMG) {
		std::cerr << "SDL Image could not be initialized" << std::endl;
		isRunning = false;
		return;
	}

	// Make alpha/transparency work
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	// Initialize prevTime to ensure correct first deltaTime
	prevTime = SDL_GetPerformanceCounter();

	// Create starting scene
	scenes.push_back(std::make_unique<Scene>(*this));
	currentScene = 0;
	scenes[currentScene]->initialize();
	
//	gameObjects.reserve(1 << 16); // Reserve memory to ensure pointer stability
//	
//	enemyManager = EnemyManager(); // Create EnemyManager

	renderManager = RenderManager(); // Create RenderManager

	//player = instantiate<Player>(vector2Df(500, 500)); // Instantiate player

	std::cout << "Initialized Game" << std::endl;
}

Game::~Game() {}

void Game::handleEvents() {
	memset(mouseInput, 0, sizeof(mouseInput)); // Reset mouseInput

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				input[event.key.keysym.scancode] = true; // Set key to true
				break;
			case SDL_KEYUP:
				input[event.key.keysym.scancode] = false; // Set key to false
				break;
			case SDL_MOUSEMOTION:
				// Update mouse position
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseInput[event.button.button] = true;
				break;
			default:
				break;
		}
	}
}

void Game::update() {
	// Calculate deltaTime
	const Uint64 nowTime = SDL_GetPerformanceCounter();
	const float deltaTime = (float)(nowTime - prevTime) / (float)SDL_GetPerformanceFrequency();
	prevTime = nowTime;

	// Call update on current scene
	scenes[currentScene]->update(deltaTime);
	// Delete objects marked for deletion
	scenes[currentScene]->updateDelete();
}

void Game::render() const {
	SDL_SetRenderDrawColor(renderer, 84, 47, 63, 255); // Set background color
	SDL_RenderClear(renderer); // Clear screen
	
	scenes[currentScene]->render(renderer); // Render scene
	renderManager.render(renderer); // Render overlays passed to RenderManager
	
	SDL_RenderPresent(renderer); // Update screen
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

#ifdef DEBUG_GIZMO
	std::cout << "\n";
#endif

	std::cout << "Game cleaned\n";
}

void Game::changeScene(const int sceneIndex) {
	currentScene = sceneIndex;
}
