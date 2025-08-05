#include "engine/resourceManager.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "SDL2/SDL_surface.h"
#include "engine/game.h"

#ifndef NDEBUG
#define ASSERT(condition, message, game)                                                 \
	do {                                                                                 \
		if (!(condition)) {                                                              \
			std::cerr << "Assertion '" #condition "' failed in " << __FILE__ << " line " \
			          << __LINE__ << ": " << message << std::endl;                       \
			game.clean();                                                                \
			std::terminate();                                                            \
		}                                                                                \
	} while (false)
#else
#define ASSERT(condition, message) \
	do {                           \
	} while (false)
#endif

namespace ResourceManager {
std::unordered_map<std::string, SDL_Texture*> loadedTextures;

SDL_Texture* LoadTexture(const std::string& filename, Game& game) {
	// Load texture if it is not already loaded
	if (!loadedTextures.count(filename)) {
// Check ASSETS_PATH is defined
#ifndef ASSETS_PATH
		std::cerr << "ERROR: ASSETS_PATH not defined." << std::endl;
		game.clean();
		std::terminate();
#endif
		const std::string path = ASSETS_PATH + filename;
		// Check that file exists
		ASSERT(std::filesystem::exists(path), "Could not load texture \"" + filename + "\"", game);
		// Store loaded textures to avoid unnecessary load calls
		SDL_Surface* surface = SDL_LoadBMP(path.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
		loadedTextures[filename] = texture;
	}

	return loadedTextures[filename];
}
}  // namespace ResourceManager
