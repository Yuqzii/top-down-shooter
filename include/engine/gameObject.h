#pragma once

#include "SDL2/SDL.h"
#include "engine/animationData.h"
#include "engine/collision.h"
#include "engine/vector2D.h"
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

// Use this inside protected section of child class to set its texture
#define SETOBJECTTEXTURE(FILE)                                                 \
  const std::string &getTextureSheet() const override {                        \
    static const std::string file = FILE;                                      \
    return file;                                                               \
  }

class Scene;

class GameObject {
public:
  GameObject();
  virtual ~GameObject() = default;

  // Initialize must be overriden to change initialization of things such as
  // collider settings
  virtual void initialize(const vector2Df &position, const Scene &scene);
  // Should be called after finishing velocity calculations
  virtual void update(Scene &scene, const float deltaTime);
  void render(SDL_Renderer *renderer) const;

  virtual void checkCollisions(const Scene &scene);
  void collisionUpdate();
  bool getUseCollision() const { return useCollision; }

  // Position and rotation
  vector2Df getPosition() const { return position; };
  vector2Df getPivotPosition() const { return pivotPosition; };
  vector2Df getMidPosition() const { return midPosition; };
  vector2Df getVelocity() const { return velocity; };
  // Returns the rotation as a direction vector
  inline vector2Df getDirection() const {
    float radians = (rotation - 90) * M_PI / 180;
    return vector2Df((float)std::cos(radians), (float)std::sin(radians));
  };

  // Collision
  Collision::Circle circleCollider;
  void addCollision(const GameObject *other);
  Collision::Types getCollisionType() const { return collisionType; }

  bool deleteObject; // When true object is deleted on next frame

protected:
  // Position and rotation
  vector2Df velocity;
  vector2Df pivotPosition;
  vector2Df midPosition;
  double rotation; // Angle of rotation

  // Collision

  // If an int exception is thrown inside this function
  // the entire collisionUpdate is aborted.
  virtual void onCollision(const GameObject &other) {}
  bool useCollision;
  Collision::Types collisionType;
  const float boundingCircle;
  std::unordered_set<const GameObject *> collisionList;

  // Animation
  bool isAnimated;      // Set true to enable animation
  float animationSpeed; // Scales all animations
  virtual void changeAnimation(const int sequenceId);
  // Use this function to define length and speed of different animations
  virtual const std::vector<AnimationData> &getAnimationData() const {
    static const std::vector<AnimationData> data;
    return data;
  }
  // Use this function to define animation events
  virtual const std::vector<AnimationEvent> &getAnimationEvents() const {
    static const std::vector<AnimationEvent> events;
    return events;
  }

  // Pivot
  SDL_Point pivot;
  vector2D pivotOffset;

  // Rendering
  virtual const std::string &getTextureSheet() const {
    static const std::string file = "default_gameobject.png";
    return file;
  };
  SDL_RendererFlip flipType;
  SDL_Rect srcRect, destRect;

  virtual std::function<void(SDL_Renderer *)> debugRender() const;

private:
  SDL_Texture *texture;

  vector2Df position;

  // Animation
  void animationUpdate(Scene &scene, const double &deltaTime);
  float animationCounter; // Keeps track of current animation frame, used as x
                          // position
  int animationSequence; // Keeps track of current animation sequence, used as y
                         // position
  int prevFrame;
};
