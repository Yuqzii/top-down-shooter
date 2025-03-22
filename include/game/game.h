#pragma once

#include <list>
#include <memory>
#include "SDL2/SDL.h"
#include "game/gameObject.h"
#include "game/vector2D.h"

class GameObject;

class Game {
public:
	Game(const char* title, int width, int height);
	~Game();

	// Function to instantiate GameObjects
	// Returns a raw pointer to the instantiated object
	template<class T>
	T* instantiate(const std::string textureSheet, const vector2Df position) {
		// Compile time check that we don't try to instantiate a non-GameObject
		static_assert(std::is_base_of<GameObject, T>(),
		"Object to instantiate must inherit from GameObject");

		// Create the new GameObject as a unique_ptr to clarify that Game has ownership
		std::unique_ptr<T> newObject = std::make_unique<T>();
		newObject->initialize(textureSheet, position, this); // Initialize GameObject
		gameObjects.push_back(std::move(newObject)); // Add GameObject to list

		return static_cast<T*>(gameObjects.back().get()); // Returns the newest GameObject, e.g. the one created now
	}
	
	// Game loop
	void handleEvents();
	void update();
	void render() const;
	void clean();

	bool running() { return isRunning; };

	// SDL stuff
	SDL_Window* getWindow() { return window; };
	SDL_Renderer* getRenderer() { return renderer; };

	bool input[256]{};
	bool mouseInput[32]{};

	double deltaTime;
	vector2D mousePos;

	std::list<std::unique_ptr<GameObject>> gameObjects; 

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint64 prevTime;
};
