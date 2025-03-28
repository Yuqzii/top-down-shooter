#include <iostream>
#include "game/game.h"

int main(int argc, char ** argv) {
#ifdef DEBUG
	std::cout << "Debug build\n";
#endif

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
