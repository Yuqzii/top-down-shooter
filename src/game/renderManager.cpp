#include <SDL2/SDL_surface.h>
#include <filesystem>
#include <string>
#include <iostream>
#include "game/renderManager.h"

#ifndef NDEBUG
#	define ASSERT(condition, message) \
	do { \
		if (!(condition)) { \
			std::cerr << "Assertion '" #condition "' failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)
#else
	#define ASSERT(condition, message) do { } while (false)
#endif

namespace RenderManager {
	namespace {
		std::unordered_map<std::string, SDL_Texture*> loadedTextures;
	}

	std::string assetsPath = "";

	SDL_Texture* LoadTexture(const std::string& filename, SDL_Renderer* renderer) {
		// Load texture if it is not already loaded
		if (!loadedTextures.count(filename)) {
			const std::string path = RenderManager::assetsPath + filename;
			// Check that file exists
			ASSERT(std::filesystem::exists(path), "Could not load texture \"" + filename + "\"");
			// Store loaded textures to avoid unnecessary load calls
			loadedTextures[filename] = IMG_LoadTexture(renderer, (path).c_str());
		}

		return loadedTextures[filename];
	}
}

