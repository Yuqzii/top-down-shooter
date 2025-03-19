#include "game/game.h"
#include "game/renderManager.h"
#include <iostream>

int main(int argc, char ** argv) {
	if (argc == 2) {
		RenderManager::assetsPath = argv[1];
	} else {
		std::cout << "Error: Invalid argument count\n";
		return 1;
	}

	Game::initialize("Cool Game", 480*3, 280*3);

	while (Game::isRunning) {
		Game::handleEvents();
		Game::update();
		Game::render();
	}

	Game::clean();

	return 0;
}
