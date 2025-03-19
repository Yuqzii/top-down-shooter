#include "game/renderManager.h"
#include "game/game.h"
#include <SDL2/SDL_surface.h>
#include <filesystem>
#include <string>
#include <iostream>

std::string RenderManager::assetsPath = "";

SDL_Texture* RenderManager::LoadTexture(std::string filename) {
	std::string path = RenderManager::assetsPath + filename;
	if (!std::filesystem::exists(path)) {
		std::cout << "File " << filename << 
			" could not be found, are you sure this is the correct name?"
			<< std::endl;
		return NULL;
	}
	return IMG_LoadTexture(Game::renderer, (path).c_str());
}
