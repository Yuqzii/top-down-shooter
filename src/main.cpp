#include "game/game.h"

int main() {
	Game game("Cool Game", 1920, 1080);

	while (game.running()) {
		game.handleEvents();
		game.update();
		game.render();
	}

	game.clean();

	return 0;
}
