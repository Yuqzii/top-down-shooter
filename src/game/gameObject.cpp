#include "game/gameObject.h"
#include "game/renderManager.h"
#include "game/game.h"

GameObject::GameObject() {
	deleteObject = false;
}

GameObject::~GameObject() {
}

void GameObject::initialize(const std::string& textureSheet, const vector2Df& startPosition,
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
	destRect.x = round(position.x);
	destRect.y = round(position.y);

	// Reset rotation
	rotation = 0;
	flipType = SDL_FLIP_NONE;

	// Initialize collider
	circleCollider.radius = (float)destRect.w / 2;
}

void GameObject::update(Game* game, const double& deltaTime) {
	destRect.x = round(position.x);
	destRect.y = round(position.y);
	
	circleCollider.position.x = round(midPosition().x);
	circleCollider.position.y = round(midPosition().y);
}

void GameObject::render(SDL_Renderer* renderer) const {
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, NULL, flipType);
	
	// Draw collider, comment line for prod
	Collision::drawCircleCollider(renderer, circleCollider);
}

vector2Df GameObject::midPosition() const {
	return vector2Df(position.x + (float)destRect.w / 2, position.y + (float)destRect.h / 2);
}
