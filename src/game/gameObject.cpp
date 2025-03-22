#include "game/gameObject.h"
#include "game/renderManager.h"
#include "game/game.h"

GameObject::GameObject() {
	deleteObject = false;
}

GameObject::~GameObject() {
}

void GameObject::initialize(const std::string textureSheet, const vector2Df startPosition,
							Game* game) {
	// Load texture
	texture = RenderManager::LoadTexture(textureSheet, game->getRenderer());

	// Initialze position
	position.x = startPosition.x;
	position.y = startPosition.y;

	// Initialize rectangles
	srcRect.h = srcRect.w = 32;
	srcRect.x = srcRect.y = 0;

	destRect.w = srcRect.w * 3; // Create global macro or sum for size instead of 3
	destRect.h = srcRect.h * 3;

	// Reset rotation
	rotation = 0;
	flipType = SDL_FLIP_NONE;
}

void GameObject::update(Game* game, const double& deltaTime) {
	destRect.x = round(position.x);
	destRect.y = round(position.y);
}

void GameObject::render(SDL_Renderer* renderer) const {
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, NULL, flipType);
}
