#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "game/collision.h"
#include "game/vector2D.h"

class Game;

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	virtual void initialize(const std::string textureSheet, const vector2Df position, Game* game);
	virtual void update(Game* game, const double& deltaTime);
	void render(SDL_Renderer* renderer) const;

	vector2Df midPosition() const;

	vector2Df position;
	double rotation; // Angle of rotation
	SDL_RendererFlip flipType;
	SDL_Rect srcRect, destRect;
	Collision::Circle circleCollider;

	bool deleteObject; // When true object is deleted on next frame
private:
	SDL_Texture* texture;
};
