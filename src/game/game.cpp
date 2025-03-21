#include <iostream>
#include <memory>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_keyboard.h"
#include "game/game.h"
#include "game/renderManager.h"
#include "game/gameObject.h"

Game::Game(const char* title, int width, int height) {
	// Check that SDL initializes
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized:\n" << SDL_GetError();
	}

	// Create window and renderer
	window = nullptr;
	renderer = nullptr;
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	
	isRunning = true;

	int flagsIMG = IMG_INIT_PNG;
	if ((IMG_Init(flagsIMG) & flagsIMG) != flagsIMG) {
		std::cout << "SDL Image could not be initialized\n";
		isRunning = false;
	}

	prevTime = SDL_GetPerformanceCounter(); // Initialize prevTime to ensure correct first deltaTime

	std::cout << "Initialized Game\n";
}

Game::~Game() {

}

void Game::addGameObject(std::shared_ptr<GameObject> gameObject) {
	gameObjects.push_back(gameObject);
}

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
				mousePos = { event.motion.x, event.motion.y }; // Update mouse position
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
	Uint64 nowTime = SDL_GetPerformanceCounter();
	deltaTime = (double)(nowTime - prevTime) 
		/ double(SDL_GetPerformanceFrequency());
	prevTime = nowTime;

	// Update all gameObjects
	for (auto& object : gameObjects) {
		object->update(this, deltaTime);
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 84, 47, 63, 255);
	SDL_RenderClear(renderer);
	
	// Render all gameObjects
	for (auto& object : gameObjects) {
		object->render(renderer);
	}

	SDL_RenderPresent(renderer); // Update screen
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned\n";
}
