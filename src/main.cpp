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

	Game game("Cool Game", 480*3, 280*3);

	while (game.running()) {
		game.handleEvents();
		game.update();
		game.render();
	}

	game.clean();

	return 0;
}
