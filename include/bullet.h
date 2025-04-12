#pragma once

#include "engine/gameObject.h"

class Game;

class Bullet : public GameObject {
public:
  void initialize(const vector2Df &startPosition, const Scene &scene) override;
  void update(Scene &scene, const float deltaTime) override;

  void checkCollisions(const Scene &scene) override;
  void onCollision(const GameObject &other) override;

  void initializeDirection(const vector2Df direction, const float rotation);

  float getDamage() const { return damage; }

protected:
  SETOBJECTTEXTURE("bullet.png");

private:
  vector2Df direction;
  vector2Df previousPosition;
  constexpr static const float speed = 1000;
  constexpr static const float damage = 25;

  constexpr static const float startTime = 4;
  float timeLeft = startTime;
};
