#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <cassert>
#include <iostream>
#include "engine/gameObject.h"
#include "engine/resourceManager.h"
#include "engine/game.h"

GameObject::GameObject() : boundingCircle(500.0f), useCollision(false) {
	deleteObject = false;

	// Initialize source rectangle (part of textureSheet that is displayed)
	// default to top left 32x32
	srcRect.h = srcRect.w = 32;
	srcRect.x = srcRect.y = 0;
	
	// Initialize destination rectangle (part of screen GameObject is displayed on)
	destRect.w = srcRect.w * 3; // Create global macro or sum for size instead of 3?
	destRect.h = srcRect.h * 3;

	pivotOffset.x = pivotOffset.y = 0;

	isAnimated = false;
	animationCounter = animationSequence = 0;
}

void GameObject::initialize(const vector2Df& startPosition, Game* game) {
	// Load texture
	texture = ResourceManager::LoadTexture(getTextureSheet(), game->getRenderer());

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
	circleCollider.position = pivotPosition;
}

void GameObject::update(Game* game, const double& deltaTime) {
	collisionList.clear(); // Make sure collisionList only contains collisions from this frame

	position += velocity * deltaTime;

	// Update render position
	destRect.x = round(position.x);
	destRect.y = round(position.y);

	// Update pivotPosition
	pivotPosition.x = pivot.x + destRect.x;
	pivotPosition.y = pivot.y + destRect.y;

	// Update midPosition
	midPosition.x = position.x + destRect.w / 2.0f;
	midPosition.y = position.y + destRect.h / 2.0f;
	midPosition = midPosition.rotateAround(
		vector2Df(destRect.x + pivot.x, destRect.y + pivot.y), rotation);

	// Update collider position
	circleCollider.position = pivotPosition;

	if (isAnimated)
		animationUpdate(deltaTime);

#ifdef DEBUG_GIZMO
	game->getRenderManager()->addRenderCall(debugRender(), this);
#endif
}

void GameObject::checkCollisions(Game* game) {
	// Get all GameObjects withing our bounding circle
	std::vector<GameObject*> closeObjects =
			game->getObjectTree().findObjectsInRange(pivotPosition, boundingCircle);

	for (GameObject* object : closeObjects) {
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (!object->useCollision || collisionList.count(object) || object == this)
			continue;

		if (Collision::checkCollision(circleCollider, object->circleCollider)) {
			addCollision(object);
			object->addCollision(this);
		}
	}
}

void GameObject::collisionUpdate() {
	for (const GameObject* object : collisionList) {
		try {
			onCollision(object);
		}
		catch (int e) { // Stop collision detection when throwing exception
			break;
		}
	}
}

void GameObject::addCollision(const GameObject* other) {
	collisionList.insert(other);
}

void GameObject::render(SDL_Renderer* renderer) const {
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flipType);
}

void GameObject::animationUpdate(const double& deltaTime) {
	// Notify if we try accessing non-existent animation
	assert(animationSequence < getAnimationData().size() && "Animation index out of range");

	const AnimationData& sequence = getAnimationData()[animationSequence];

	// Update frame
	animationCounter += sequence.speed * animationSpeed * deltaTime;
	// Check for looping
	if (animationCounter >= sequence.length) {
		animationCounter -= sequence.length; // -= length so that the animation plays at same speed
	}
	int frame = std::trunc(animationCounter);

	srcRect.x = frame * 32;
	srcRect.y = animationSequence * 32;
}

std::function<void(SDL_Renderer*)> GameObject::debugRender() const {
	// Return lambda with debug render stuff
	return [this](SDL_Renderer* renderer) {
		// Collider
		if (useCollision)
			Collision::drawCircleCollider(renderer, circleCollider);
		SDL_RenderDrawPoint(renderer, pivot.x + destRect.x, pivot.y + destRect.y);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawPoint(renderer, midPosition.x, midPosition.y);
		SDL_RenderDrawLine(renderer, pivotPosition.x, pivotPosition.y,
						pivotPosition.x + velocity.x * 0.1,
						pivotPosition.y + velocity.y * 0.1
		);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &destRect);
	};
}
