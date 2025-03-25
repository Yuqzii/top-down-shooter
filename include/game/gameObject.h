#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "game/collision.h"
#include "game/vector2D.h"
#include "game/animationData.h"

// Use this inside protected section of child class to set its texture
#define SETOBJECTTEXTURE(FILE) \
const std::string& getTextureSheet() const override { \
	static const std::string file = FILE; \
	return file; \
}

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
	// Returns the rotation as a direction vector
	inline vector2Df getDirection() const {
		float radians = (rotation - 90) * M_PI / 180;
		return vector2Df((float)std::cos(radians), (float)std::sin(radians));
	};

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
	bool isAnimated; // Set true to enable animation
	int animationSequence; // Keeps track of current animation sequence, used as y position
	// Use this function to define length and speed of different animations
	virtual const std::vector<AnimationData>& getAnimationData() const {
		static const std::vector<AnimationData> data;
		return data;
	}

	// Pivot
	SDL_Point pivot;
	vector2D pivotOffset;

	// Rendering
	virtual const std::string& getTextureSheet() const { 
		static const std::string file = "default_gameobject.png";
		return file; 
	};
	SDL_RendererFlip flipType;
	SDL_Rect srcRect, destRect;
	
private:
	SDL_Texture* texture;
	
	// Animation
	void animationUpdate(const double& deltaTime);
	float animationCounter; // Keeps track of current animation frame, used as x position
};
