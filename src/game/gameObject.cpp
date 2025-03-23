#include "game/gameObject.h"
#include "game/renderManager.h"
#include "game/game.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

GameObject::GameObject() {
	deleteObject = false;

	pivotOffset.x = pivotOffset.y = 0;
}

GameObject::~GameObject() {
}

void GameObject::initialize(const std::string& textureSheet, const vector2Df& startPosition,
							Game* game) {
	// Load texture
	texture = RenderManager::LoadTexture(textureSheet, game->getRenderer());
	
	// Initialize source rectangle (part of textureSheet that is displayed)
	// default to top left 32x32
	srcRect.h = srcRect.w = 32;
	srcRect.x = srcRect.y = 0;
	
	// Initialize destination rectangle (part of screen GameObject is displayed on)
	destRect.w = srcRect.w * 3; // Create global macro or sum for size instead of 3?
	destRect.h = srcRect.h * 3;

	// Initialize pivot
	pivot.x = destRect.w / 2 + pivotOffset.x;
	pivot.y = destRect.h / 2 + pivotOffset.y;

	// Initialze position
	pivotPosition = startPosition;

	position.x = pivotPosition.x - pivot.x;
	position.y = pivotPosition.y - pivot.y;

	destRect.x = round(position.x);
	destRect.y = round(position.y);

	// Reset rotation
	rotation = 0;
	flipType = SDL_FLIP_NONE;

	// Initialize collider
	circleCollider.radius = (float)destRect.w / 2;
}

void GameObject::update(Game* game, const double& deltaTime) {
	// Update render position
	destRect.x = round(position.x);
	destRect.y = round(position.y);

	// Update pivotPosition
	pivotPosition.x = pivot.x + destRect.x;
	pivotPosition.y = pivot.y + destRect.y;

	// Update midPosition
	midPosition.x = position.x + (float)destRect.w / 2;
	midPosition.y = position.y + (float)destRect.h / 2;
	midPosition = midPosition.rotateAround(
		vector2Df(destRect.x + pivot.x, destRect.y + pivot.y), rotation);

	// Update collider position
	circleCollider.position = pivotPosition;
}

void GameObject::render(SDL_Renderer* renderer) const {
	// Render without extra pivot calculation
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flipType);
	
	// Draw collider, comment line for prod
	#ifdef DEBUG
	Collision::drawCircleCollider(renderer, circleCollider);
	SDL_RenderDrawPoint(renderer, pivot.x + destRect.x, pivot.y + destRect.y);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawPoint(renderer, midPosition.x, midPosition.y);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	#endif
	#ifdef DRAWRECT
	SDL_RenderDrawRect(renderer, &destRect);
	#endif
}

inline vector2Df GameObject::getDirection() const {
	float radians = (rotation - 90) * M_PI / 180;
	return vector2Df((float)std::cos(radians), (float)std::sin(radians));
}
