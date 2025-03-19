#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "game/vector2D.h"

class GameObject {

public:
	GameObject(std::string textureSheet, vector2Df position);
	~GameObject();

	void update(double deltaTime);
	void render(SDL_Renderer* renderer);
private:
	vector2Df position;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};
