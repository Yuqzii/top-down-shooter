#include "engine/game.h"
#include <iostream>

int main(int argc, char **argv) {
#ifdef DEBUG_GIZMO
  std::cout << "Debug gizmos enabled" << std::endl;
#endif

  Game game("Cool Game", 480 * 3, 280 * 3); // Create Game instance

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
