#pragma once

#include <string>
#include <unordered_map>
#include "SDL2/SDL_image.h"

namespace RenderManager {
	extern std::string assetsPath;
	SDL_Texture* LoadTexture(const std::string filename, SDL_Renderer* renderer);
}
