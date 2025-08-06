#pragma once

#include <string>
#include <unordered_map>

#include "SDL2/SDL_render.h"

class Game;

class ResourceManager {
public:
	ResourceManager(std::string&& assetsPath);

	SDL_Texture* loadTexture(const std::string_view filename, Game& game);
	void destroyTextures();

private:
	const std::string assetsPath;
	std::unordered_map<std::string_view, SDL_Texture*> loadedTextures;
};
