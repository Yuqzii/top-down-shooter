#include "engine/gameObject.h"

#include <cassert>

#include "engine/game.h"
#include "engine/resourceManager.h"
#include "engine/scene.h"

// Initialize source rectangle (part of textureSheet that is displayed)
// default to top left 32x32

// Initialize destination rectangle (part of screen GameObject is displayed on)
// Create global macro or sum for size instead of 3?
GameObject::GameObject(std::unique_ptr<Collider> collider_, const Vec2& srcRectSize)
	: deleteObject{false},
	  size{1, 1},
	  baseSize{srcRectSize},
	  srcRect{0, 0, (int)srcRectSize.x, (int)srcRectSize.y},
	  destRect{0, 0, srcRect.w * Game::pixelSize, srcRect.h * Game::pixelSize},
	  velocity{},
	  pivotOffset{0, 0},
	  isAnimated{false},
	  animationCounter{0},
	  animationSequence{0},
	  prevFrame{0},
	  collider{std::move(collider_)},
	  rotation{0},
	  flipType{SDL_FLIP_NONE},
	  isStatic{false},
	  renderObject{true} {}

GameObject::GameObject(const Vec2& srcRectSize) : GameObject{nullptr, srcRectSize} {}

GameObject::GameObject() : GameObject{nullptr} {}

void GameObject::initialize(const Scene& scene, const Vec2& startPosition) {
	// Load texture
	texture = ResourceManager::LoadTexture(getTextureSheet(), scene.getGame());

	// Initialize pivot
	pivot.x = (float)destRect.w / 2 + pivotOffset.x;
	pivot.y = (float)destRect.h / 2 + pivotOffset.y;

	// Initialze position
	position = startPosition;

	const Vec2 camPos = scene.getCam().getPos();
	screenPosition.x = position.x - camPos.x;
	screenPosition.y = position.y - camPos.y;
	// Update render positon
	renderPosition.x = screenPosition.x - pivot.x;
	renderPosition.y = screenPosition.y - pivot.y;

	destRect.x = round(renderPosition.x);
	destRect.y = round(renderPosition.y);
}

void GameObject::update(Scene& scene, const float deltaTime) {
	if (!isStatic) {
		position += velocity * deltaTime;
	}

	const Vec2 camPos = scene.getCam().getPos();
	screenPosition.x = position.x - camPos.x;
	screenPosition.y = position.y - camPos.y;
	// Update render positon
	renderPosition.x = screenPosition.x - pivot.x;
	renderPosition.y = screenPosition.y - pivot.y;

	// Update render rectangle
	destRect.x = round(renderPosition.x);
	destRect.y = round(renderPosition.y);

	if (isAnimated) animationUpdate(scene, deltaTime);

#ifdef DEBUG_GIZMO
	scene.getGame().getRenderManager().addRenderCall(debugRender(), this);
#endif
}

void GameObject::setSize(const Vec2& newSize) {
	size = newSize;

	// Update render rect
	destRect.w = size.x * baseSize.x * Game::pixelSize;
	destRect.h = size.y * baseSize.y * Game::pixelSize;

	// Update pivot
	pivot.x = (float)destRect.w / 2 + pivotOffset.x * size.x;
	pivot.y = (float)destRect.h / 2 + pivotOffset.y * size.y;
}

void GameObject::render(SDL_Renderer* renderer) const {
	if (!renderObject) return;
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
		srcRect.x = frame + frame * 32;
		srcRect.y = animationSequence + animationSequence * 32;
	}

	prevFrame = frame;
}

void GameObject::changeAnimation(const int sequenceId) {
	// Do nothing if this is already the current animation
	if (animationSequence == sequenceId) return;

	animationSequence = sequenceId;
	animationCounter = 0;
}

std::function<void(Scene&)> GameObject::debugRender() const {
	// Return lambda with debug render stuff
	return [this](Scene& scene) {
		SDL_Renderer* renderer = scene.getGame().getRenderer();
		// Collider
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		const Vec2 camPos = scene.getCam().getPos();
		switch (collider->getCollisionType()) {
			using enum Collision::Types;
			case CIRCLE: {
				CircleCollider* circleCollider = static_cast<CircleCollider*>(collider.get());
				Collision::drawCircleCollider(
					renderer, Collision::Circle{circleCollider->circle.position - camPos,
												circleCollider->circle.radius});
				break;
			}
			case LINE: {
				LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
				SDL_RenderDrawLineF(renderer, lineCollider->line.start.x - camPos.x,
									lineCollider->line.start.y - camPos.y,
									lineCollider->line.end.x - camPos.x,
									lineCollider->line.end.y - camPos.y);
			}
			case POINT: {
				PointCollider* pointCollider = static_cast<PointCollider*>(collider.get());
				SDL_RenderDrawPoint(renderer, pointCollider->point.x - camPos.x,
									pointCollider->point.y - camPos.y);
			}
		}
		SDL_RenderDrawPoint(renderer, screenPosition.x, screenPosition.y);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, screenPosition.x, screenPosition.y,
						   screenPosition.x + velocity.x * 0.1,
						   screenPosition.y + velocity.y * 0.1);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &destRect);
	};
}
