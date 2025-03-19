#include "game/renderManager.h"
#include <SDL2/SDL_surface.h>
#include <filesystem>
#include <string>
#include <iostream>

std::string RenderManager::assetsPath = "";

SDL_Texture* RenderManager::LoadTexture(std::string filename, SDL_Renderer* renderer) {
	std::string path = RenderManager::assetsPath + filename;
	if (!std::filesystem::exists(path)) {
		std::cout << "File " << filename << 
			" could not be found, are you sure this is the correct name?"
			<< std::endl;
		return NULL;
	}
	return IMG_LoadTexture(renderer, (path).c_str());
}
