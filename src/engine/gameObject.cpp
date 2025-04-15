#include "engine/gameObject.h"

#include <cassert>
#include <iostream>

#include "engine/game.h"
#include "engine/resourceManager.h"
#include "engine/scene.h"

// Initialize source rectangle (part of textureSheet that is displayed)
// default to top left 32x32

// Initialize destination rectangle (part of screen GameObject is displayed on)
// Create global macro or sum for size instead of 3?
GameObject::GameObject()
	: deleteObject{false},
	  srcRect{0, 0, 32, 32}, destRect{0, 0, srcRect.w * 3, srcRect.h * 3},
	  pivotOffset{0, 0}, isAnimated{false}, animationCounter{0}, animationSequence{0}, prevFrame{0},
	  boundingCircle{500.0f}, useCollision{false}, collisionList{}, collisionType{} {}

void GameObject::initialize(const vector2Df& startPosition, const Scene& scene) {
	// Load texture
	texture = ResourceManager::LoadTexture(getTextureSheet(), scene.getGame().getRenderer());

	// Initialize pivot
	pivot.x = destRect.w / 2 + pivotOffset.x;
	pivot.y = destRect.h / 2 + pivotOffset.y;

	// Initialze position
	position = startPosition;

	renderPosition.x = position.x - pivot.x;
	renderPosition.y = position.y - pivot.y;

	destRect.x = round(renderPosition.x);
	destRect.y = round(renderPosition.y);

	// Reset rotation
	rotation = 0;
	flipType = SDL_FLIP_NONE;

	// Initialize collider
	circleCollider.radius = (float)destRect.w / 2;
	circleCollider.position = position;
}

void GameObject::update(Scene& scene, const float deltaTime) {
	collisionList.clear();	// Make sure collisionList only contains collisions from this frame

	renderPosition += velocity * deltaTime;

	// Update render position
	destRect.x = round(renderPosition.x);
	destRect.y = round(renderPosition.y);

	// Update pivotPosition
	position.x = pivot.x + destRect.x;
	position.y = pivot.y + destRect.y;

	// Update collider position
	circleCollider.position = position;

	if (isAnimated) animationUpdate(scene, deltaTime);

#ifdef DEBUG_GIZMO
	scene.getGame().getRenderManager().addRenderCall(debugRender(), this);
#endif
}

void GameObject::checkCollisions(const Scene& scene) {
	// Point collisions are not checking collision with others
	if (collisionType == Collision::Types::POINT) return;

	std::vector<GameObject*> closeObjects;
	try {
		// Get all GameObjects withing our bounding circle
		closeObjects = scene.getObjectTree().findObjectsInRange(position, boundingCircle);
	} catch (int e) {
		std::cerr << "Exception " << e << " when checking collisions. Tree was likely not built.\n";
	}

	for (GameObject* object : closeObjects) {
		// No need to check collision if object is not collideable,
		// or we know we have already collided,
		// or if it is "colliding" with itself.
		if (!object->useCollision || collisionList.count(object) || object == this) continue;

		switch (object->collisionType) {
			using namespace Collision;
			using enum Types;

			case CIRCLE:
				if (Collision::checkCollision(circleCollider, object->circleCollider)) {
					addCollision(object);
					object->addCollision(this);
				}
				break;

			case POINT:
				if (Collision::checkCollision(object->getPosition(), circleCollider)) {
					addCollision(object);
				}
				break;
		}
	}
}

void GameObject::collisionUpdate() {
	for (const GameObject* object : collisionList) {
		if (object == nullptr)	// Ensure that object exists
			continue;

		try {
			onCollision(*object);
		} catch (int e) {
			// Stop collision detection when throwing exception
			break;
		}
	}
}

void GameObject::addCollision(const GameObject* other) { collisionList.insert(other); }

void GameObject::render(SDL_Renderer* renderer) const {
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flipType);
}

void GameObject::animationUpdate(Scene& scene, const double& deltaTime) {
	// Notify if we try accessing non-existent animation
	assert(animationSequence < getAnimationData().size() && "Animation index out of range");

	const AnimationData& sequence = getAnimationData()[animationSequence];
	const float prevAnimationCounter = animationCounter;

	// Update frame
	animationCounter += sequence.speed * animationSpeed * deltaTime;

	// Check animation events
	for (const AnimationEvent& event : getAnimationEvents()) {
		// Call event if animation sequence matches event,
		// and the previous frame was before the event and the current is after.
		if (event.sequenceId == animationSequence && event.time <= animationCounter &&
			event.time >= prevAnimationCounter) {
			event.event(scene);
		}
	}

	// Check for looping
	if (animationCounter >= sequence.length) {
		animationCounter -= sequence.length;  // -= length so that the animation plays at same speed
	}
	const int frame = std::trunc(animationCounter);

	if (frame != prevFrame) {
		srcRect.x = frame * 32;
		srcRect.y = animationSequence * 32;
	}

	prevFrame = frame;
}

void GameObject::changeAnimation(const int sequenceId) {
	// Do nothing if this is already the current animation
	if (animationSequence == sequenceId) return;

	animationSequence = sequenceId;
	animationCounter = 0;
}

std::function<void(SDL_Renderer*)> GameObject::debugRender() const {
	// Return lambda with debug render stuff
	return [this](SDL_Renderer* renderer) {
		// Collider
		if (useCollision) Collision::drawCircleCollider(renderer, circleCollider);
		SDL_RenderDrawPoint(renderer, pivot.x + destRect.x, pivot.y + destRect.y);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, position.x, position.y, position.x + velocity.x * 0.1,
						   position.y + velocity.y * 0.1);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &destRect);
	};
}
