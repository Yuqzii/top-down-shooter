#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "SDL2/SDL_render.h"
#include "engine/animationData.h"
#include "engine/collision.h"
#include "engine/vector2D.h"

// Use this inside protected section of child class to set its texture
#define SETOBJECTTEXTURE(FILE)                            \
	const std::string& getTextureSheet() const override { \
		static const std::string file = FILE;             \
		return file;                                      \
	}

class Scene;

class GameObject {
public:
	GameObject(std::unique_ptr<Collider> collider, const Vec2& srcRectSize = {32, 32});
	GameObject(const Vec2& srcRectSize);
	GameObject();
	virtual ~GameObject() = default;

	virtual void initialize(const Vec2& position, const Scene& scene);
	// Should be called after finishing velocity calculations
	virtual void update(Scene& scene, const float deltaTime);
	void render(SDL_Renderer* renderer) const;

	// Position and rotation
	Vec2 getRenderPosition() const { return renderPosition; }
	Vec2 getPosition() const { return position; }
	Vec2 getVelocity() const { return velocity; }
	Vec2 getSize() const { return size; }
	// Returns the rotation as a direction vector
	inline Vec2 getDirection() const {
		float radians = (rotation - 90) * M_PI / 180;
		return Vec2((float)std::cos(radians), (float)std::sin(radians));
	};

	//----- COLLISION -----//

	Collider* getCollider() const { return collider.get(); }
	// If an int exception is thrown inside this function
	// the entire collisionUpdate is aborted.
	virtual void onCollision(const Collision::Event& event, Scene& scene) {}
	bool getIsStatic() const { return isStatic; }

	bool deleteObject;	// When true object is deleted on next frame

protected:
	// Position and rotation
	Vec2 velocity;
	Vec2 position;
	double rotation;  // Angle of rotation

	void setSize(const Vec2& newSize);

	std::unique_ptr<Collider> collider;
	bool isStatic;

	// Animation
	bool isAnimated;	   // Set true to enable animation
	float animationSpeed;  // Scales all animations
	virtual void changeAnimation(const int sequenceId);
	// Use this function to define length and speed of different animations
	virtual const std::vector<AnimationData>& getAnimationData() const {
		static const std::vector<AnimationData> data;
		return data;
	}
	// Use this function to define animation events
	virtual const std::vector<AnimationEvent>& getAnimationEvents() const {
		static const std::vector<AnimationEvent> events;
		return events;
	}
	bool renderObject;

	// Pivot
	SDL_Point pivot;
	Vec2 pivotOffset;

	// Rendering
	virtual const std::string& getTextureSheet() const {
		static const std::string file = "default_gameobject.png";
		return file;
	};
	SDL_RendererFlip flipType;
	SDL_Rect srcRect, destRect;

	virtual std::function<void(SDL_Renderer*)> debugRender() const;

private:
	SDL_Texture* texture;

	const Vec2 baseSize;
	Vec2 size;

	Vec2 renderPosition;

	// Animation
	void animationUpdate(Scene& scene, const double& deltaTime);
	float animationCounter;	 // Keeps track of current animation frame, used as x position
	int animationSequence;	 // Keeps track of current animation sequence, used as y position
	int prevFrame;
};
