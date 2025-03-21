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

	virtual void initialize(std::string textureSheet, vector2Df position, Game* game);
	virtual void update(Game* game, double deltaTime);
	void render(SDL_Renderer* renderer);

	vector2Df position;
private:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};
