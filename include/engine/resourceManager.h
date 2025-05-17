#pragma once

#include <string>

#include "SDL2/SDL_render.h"

class Game;

namespace ResourceManager {
SDL_Texture* LoadTexture(const std::string& filename, Game& game);
}
