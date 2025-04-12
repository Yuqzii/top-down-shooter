#include "engine/resourceManager.h"

#include <SDL2/SDL_surface.h>

#include <filesystem>
#include <iostream>
#include <string>

#ifndef NDEBUG
#define ASSERT(condition, message)                                                       \
	do {                                                                                 \
		if (!(condition)) {                                                              \
			std::cerr << "Assertion '" #condition "' failed in " << __FILE__ << " line " \
					  << __LINE__ << ": " << message << std::endl;                       \
			std::terminate();                                                            \
		}                                                                                \
	} while (false)
#else
#define ASSERT(condition, message) \
	do {                           \
	} while (false)
#endif

namespace ResourceManager {
namespace {
std::unordered_map<std::string, SDL_Texture*> loadedTextures;
}

SDL_Texture* LoadTexture(const std::string& filename, SDL_Renderer* renderer) {
	// Load texture if it is not already loaded
	if (!loadedTextures.count(filename)) {
// Check ASSETS_PATH is defined
#ifndef ASSETS_PATH
		std::cerr << "ERROR: ASSETS_PATH not defined." << std::endl;
		std::terminate();
#define ASSETS_PATH ""	// Just here to hide lsp error (not actual error)
#endif

		const std::string path = ASSETS_PATH + filename;
		// Check that file exists
		ASSERT(std::filesystem::exists(path), "Could not load texture \"" + filename + "\"");
		// Store loaded textures to avoid unnecessary load calls
		loadedTextures[filename] = IMG_LoadTexture(renderer, (path).c_str());
	}

	return loadedTextures[filename];
}
}  // namespace ResourceManager
