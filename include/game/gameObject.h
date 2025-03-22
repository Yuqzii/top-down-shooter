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
	virtual ~GameObject();

	virtual void initialize(const std::string textureSheet, const vector2Df position, Game* game);
	virtual void update(Game* game, const double& deltaTime);
	void render(SDL_Renderer* renderer) const;

	vector2Df midPosition() const { 
		return vector2Df(position.x + destRect.w / 2, position.y + destRect.h / 2);
	};

	vector2Df position;
	double rotation; // Angle of rotation
	SDL_RendererFlip flipType;

	bool deleteObject; // When true object is deleted on next frame
private:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};
