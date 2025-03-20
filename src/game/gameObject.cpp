#include "game/gameObject.h"
#include "game/renderManager.h"
#include "game/game.h"

GameObject::GameObject() {
}

GameObject::~GameObject() {
}

void GameObject::initialize(std::string textureSheet, vector2Df startPosition, Game* game) {
	// Load texture
	texture = RenderManager::LoadTexture(textureSheet, game->getRenderer());

	// Update Game class
	game->addGameObject(shared_from_this());

	// Initialze position
	position.x = startPosition.x;
	position.y = startPosition.y;

	// Initialize rectangles
	srcRect.h = srcRect.w = 32;
	srcRect.x = srcRect.y = 0;

	destRect.w = srcRect.w * 3; // Create global macro or sum for size instead of 3
	destRect.h = srcRect.h * 3;
}

void GameObject::update(double deltaTime) {
	destRect.x = round(position.x);
	destRect.y = round(position.y);
}

void GameObject::render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
