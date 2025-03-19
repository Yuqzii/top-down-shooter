#include "game/game.h"
#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "game/renderManager.h"

Game::Game(const char* title, int width, int height) {
	window = nullptr;
	renderer = nullptr;

	// Check that SDL initializes
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized:\n" << SDL_GetError();
	}

	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	isRunning = true;

	int flagsIMG = IMG_INIT_PNG;
	if ((IMG_Init(flagsIMG) & flagsIMG) != flagsIMG) {
		std::cout << "SDL Image could not be initialized\n";
		isRunning = false;
	}

	// Testing stuff
	playerTex = RenderManager::LoadTexture("Player.png", renderer);
	playerRect.x = 400;
	playerRect.y = 400;
	playerRect.w = 100;
	playerRect.h = 100;

	std::cout << "Initialized Game\n";
}

Game::~Game() {

}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}

}

void Game::update() {
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	// Add stuff to render
	SDL_RenderCopy(renderer, playerTex, NULL, &playerRect);
	
	SDL_RenderPresent(renderer);

}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned\n";
}
