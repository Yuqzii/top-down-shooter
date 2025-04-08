#include <iostream>
#include "engine/engine.h"
#include "SDL2/SDL_image.h"

void Engine::initialize(const char* title, const int width, const int height) {
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

	prevTime = SDL_GetPerformanceCounter(); // Initialize prevTime to ensure correct first deltaTime
	
	gameObjects.reserve(1 << 16); // Reserve memory to ensure pointer stability
	
	enemyManager = EnemyManager(); // Create EnemyManager

	renderManager = RenderManager(); // Create RenderManager

	player = instantiate<Player>(vector2Df(500, 500)); // Instantiate player

	std::cout << "Initialized Game" << std::endl;

