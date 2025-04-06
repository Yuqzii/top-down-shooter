#include <climits>
#include <iostream>
#include <memory>
#include <ranges>
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
	
	enemyManager = EnemyManager(); // Create EnemyManager

	renderManager = RenderManager(); // Create RenderManager

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

	renderManager.resetCallCnt();
	updateObjectTree(); // Update the object tree

	// Update all GameObjects
	for (auto& object : gameObjects) {
		object->update(this, deltaTime);
	}

	// Check for collisions after all GameObjects are updated
	for (auto& object : gameObjects) {
		object->checkCollisions(this);
	}

	// Update GameObjects according to registered collisions
	for (auto& object : gameObjects) {
		object->collisionUpdate();
	}

	renderManager.update(); // Remove objects marked for deletion from RenderManager
	enemyManager.update(this, deltaTime); // Remove objects marked for deletion from EnemyManager

	// Delete objects marked for deletion
	for (auto it = gameObjects.begin(); it != gameObjects.end();) {
		if (it->get()->deleteObject) {
			it = gameObjects.erase(it); // Delete GameObject
		}
		else it++;
	}

#ifdef DEBUG_GIZMO
	std::cout << "\rFPS: " << 1 / deltaTime;
#endif
}

void Game::render() const {
	SDL_SetRenderDrawColor(renderer, 84, 47, 63, 255); // Set background color
	SDL_RenderClear(renderer);
	
	// Render all gameObjects
	for (auto& object : gameObjects) {
		object->render(renderer);
	}

	renderManager.render(renderer); // Render everything passed to RenderManager

	SDL_RenderPresent(renderer); // Update screen
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

#ifdef DEBUG_GIZMO
	std::cout << "\n";
#endif

	std::cout << "Game Cleaned\n";
}

void Game::updateObjectTree() {
	objectTree = Tree2D(); // Create new tree

	// Create vector of raw pointers from unique_ptr vector
	auto objectsRange = gameObjects
			| std::views::transform(
			[](const std::unique_ptr<GameObject>& ptr) -> GameObject* { return ptr.get(); });
	const std::vector<GameObject*> rawObjects(objectsRange.begin(), objectsRange.end());

	// Create the tree from the GameObject vector
	objectTree.initializeWithList(rawObjects);
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
