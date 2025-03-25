#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "game/collision.h"
#include "game/vector2D.h"

class Game;

class GameObject {
public:
	GameObject();

	// Initialize must be overriden to change initialization of things such as collider settings
	virtual void initialize(const vector2Df& position, Game* game);
	virtual void update(Game* game, const double& deltaTime);
	void render(SDL_Renderer* renderer) const;

	// Position and rotation
	vector2Df getPosition() const { return position; };
	vector2Df getPivotPosition() const { return pivotPosition; };
	vector2Df getMidPosition() const { return midPosition; };
	vector2Df getDirection() const; // Returns the rotation as a direction vector

	// Collision
	Collision::Circle circleCollider;

	bool deleteObject; // When true object is deleted on next frame

protected:
	// Position and rotation
	vector2Df position;
	vector2Df pivotPosition;
	vector2Df midPosition;
	double rotation; // Angle of rotation
	
	// Animation
	bool isAnimated;
	float animationCounter; // Keeps track of current animation frame
	
	// Pivot
	SDL_Point pivot;
	vector2D pivotOffset;

	// Rendering
	virtual std::string getTextureSheet() const { return "default_gameobject.png"; };
	SDL_RendererFlip flipType;
	SDL_Rect srcRect, destRect;
	
private:
	SDL_Texture* texture;
};
