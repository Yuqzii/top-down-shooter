#include "game/game.h"

Game *game = nullptr;

int main() {
	game = new Game("Cool Game", 1920, 1080);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	delete game;
	return 0;
}
