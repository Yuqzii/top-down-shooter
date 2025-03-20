#pragma once

#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include "game/game.h"
#include "game/vector2D.h"

class Game;

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject();
	~GameObject();

	void initialize(std::string textureSheet, vector2Df position, Game* game);
	void update(double deltaTime);
	void render(SDL_Renderer* renderer);
private:
	vector2Df position;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};
