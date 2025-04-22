#pragma once

#include <string>

#include "SDL2/SDL_render.h"

namespace ResourceManager {
SDL_Texture* LoadTexture(const std::string& filename, SDL_Renderer* renderer);
}
