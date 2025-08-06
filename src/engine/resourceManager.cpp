#include "engine/resourceManager.h"

#include <filesystem>
#include <iostream>
#include <string>

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

ResourceManager::ResourceManager(std::string&& assetsPath) : assetsPath{std::move(assetsPath)} {}

SDL_Texture* ResourceManager::loadTexture(const std::string_view filename, Game& game) {
	// Load texture if it is not already loaded
	if (!loadedTextures.count(filename)) {
		std::string path{assetsPath};
		path += filename;
		// Check that file exists
		ASSERT(std::filesystem::exists(path),
		       "Could not load texture \"" + static_cast<std::string>(filename) + "\"", game);
		// Store loaded textures to avoid unnecessary load calls
		SDL_Surface* surface = SDL_LoadBMP(path.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
		loadedTextures[filename] = texture;
		SDL_FreeSurface(surface);
	}

	return loadedTextures[filename];
}

void ResourceManager::destroyTextures() {
	for (auto [filename, texture] : loadedTextures) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	loadedTextures.clear();
}
