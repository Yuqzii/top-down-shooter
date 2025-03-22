#include "game/game.h"
#include "game/gameObject.h"
#include "player.h"
#include "game/renderManager.h"
#include <iostream>

int main(int argc, char ** argv) {
	if (argc == 2) {
		RenderManager::assetsPath = argv[1];
	} else {
		std::cerr << "Error: Invalid argument count\n";
		return 1;
	}

	Game game("Cool Game", 480*3, 280*3); // Create Game instance

	// Game loop
	while (game.running()) {
		game.handleEvents();
		game.update();
		game.render();
	}

	// Game closed
	game.clean();

	return 0;
}
