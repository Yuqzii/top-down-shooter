#pragma once

#include "SDL2/SDL_image.h"
#include <string>
#include <unordered_map>

namespace ResourceManager {
SDL_Texture *LoadTexture(const std::string &filename, SDL_Renderer *renderer);
}
