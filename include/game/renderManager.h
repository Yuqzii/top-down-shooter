#include "SDL2/SDL_image.h"
#include <string>

namespace RenderManager {
	extern std::string assetsPath;
	SDL_Texture* LoadTexture(std::string filename, SDL_Renderer* renderer);
}
