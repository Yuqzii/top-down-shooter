#include "game/renderManager.h"
#include <SDL2/SDL_surface.h>

SDL_Texture* RenderManager::LoadTexture(const char* filename, SDL_Renderer* renderer) {
	SDL_Surface* image = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	return texture;
}
