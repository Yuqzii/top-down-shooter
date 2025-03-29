#include <climits>
#include <iostream>
#include <memory>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_keyboard.h"
#include "game/game.h"
#include "bullet.h"
#include "enemy.h"

Game::Game(const char* title, int width, int height) {
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
	
	enemySpawner = EnemySpawner(); // Create EnemySpawner

	uiManager = UI::UIManager(); // Create UIManager

	player = instantiate<Player>(vector2Df(500, 500)); // Instantiate player

	std::cout << "Initialized Game" << std::endl;
}

Game::~Game() {

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
	Uint64 nowTime = SDL_GetPerformanceCounter();
	deltaTime = (double)(nowTime - prevTime) 
		/ (double)SDL_GetPerformanceFrequency();
	prevTime = nowTime;

	uiManager.update(); // Resets uiManager call count, does NOT update UI Widgets

	// Update all GameObjects
	for (auto& object : gameObjects) {
		object->update(this, deltaTime);
	}

	// Update EnemySpawner list
	enemySpawner.update(this, deltaTime);

	// Delete objects marked for deletion
	for (auto it = gameObjects.begin(); it != gameObjects.end();) {
		if (it->get()->deleteObject) {
			it = gameObjects.erase(it); // Delete GameObject
		}
		else it++;
	}
}

void Game::render() const {
	SDL_SetRenderDrawColor(renderer, 84, 47, 63, 255); // Set background color
	SDL_RenderClear(renderer);
	
	// Render all gameObjects
	for (auto& object : gameObjects) {
		object->render(renderer);
	}

	uiManager.render(renderer); // Render UI Widgets

	SDL_RenderPresent(renderer); // Update screen
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game Cleaned\n";
}

// Function to instantiate GameObjects, returns raw pointer to instantiated object
template<class T>
T* Game::instantiate(const vector2Df& position) {
	// Compile time check that we don't try to instantiate a non-GameObject
	static_assert(std::is_base_of<GameObject, T>(),
	"Object to instantiate must inherit from GameObject");

	// Create the new GameObject as a unique_ptr to clarify that Game has ownership
	std::unique_ptr<T> newObject = std::make_unique<T>();
	newObject->initialize(position, this); // Initialize GameObject
	gameObjects.push_back(std::move(newObject)); // Add GameObject to list
	
	// Returns the newest GameObject, e.g. the one created now
	return static_cast<T*>(gameObjects.back().get());
}
// Create all valid templates
template GameObject* Game::instantiate<GameObject>(const vector2Df& position);
template Player* Game::instantiate<Player>(const vector2Df& position);
template Bullet* Game::instantiate<Bullet>(const vector2Df& position);
template Enemy* Game::instantiate<Enemy>(const vector2Df& position);
