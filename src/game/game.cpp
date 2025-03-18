#include "game/game.h"
#include <iostream>

Game::Game(const char* title, int width, int height) {
	window = nullptr;
	renderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized: " << SDL_GetError();
	}

	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	isRunning = true;
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
	SDL_RenderClear(renderer);
	
	// Add stuff to render
	SDL_RenderDrawPoint(renderer, 500, 500);
	
	SDL_RenderPresent(renderer);

}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned\n";
}
