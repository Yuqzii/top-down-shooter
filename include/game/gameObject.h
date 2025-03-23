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

	// Initialize must be overriden to change initialization of things such as collider settings
	virtual void initialize(const std::string& textureSheet, const vector2Df& position, Game* game);
	virtual void update(Game* game, const double& deltaTime);
	void render(SDL_Renderer* renderer) const;

	vector2Df getDirection() const;

	vector2Df position;
	vector2Df pivotPosition;
	vector2Df midPosition;
	double rotation; // Angle of rotation
	SDL_RendererFlip flipType;
	SDL_Rect srcRect, destRect;
	SDL_Point pivot;
	vector2D pivotOffset;
	Collision::Circle circleCollider;

	bool deleteObject; // When true object is deleted on next frame
private:
	SDL_Texture* texture;
};
