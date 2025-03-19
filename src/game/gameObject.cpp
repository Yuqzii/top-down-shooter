#include "game/gameObject.h"
#include "game/renderManager.h"
#include "game/game.h"

GameObject::GameObject(std::string textureSheet, SDL_Renderer* renderer, 
		vector2Df startPosition) : position(startPosition.x, startPosition.y) {
	
	// Load texture
	texture = RenderManager::LoadTexture(textureSheet, renderer);

	// Initialize rectangles
	srcRect.h = srcRect.w = 32;
	srcRect.x = srcRect.y = 0;

	destRect.w = srcRect.w * 3; // Create global macro or sum for size instead of 3
	destRect.h = srcRect.h * 3;
}

GameObject::~GameObject() {
}

void GameObject::update(double deltaTime) {
	destRect.x = round(position.x);
	destRect.y = round(position.y);
}

void GameObject::render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
