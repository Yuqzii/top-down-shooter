#pragma once

#include "engine/UI/background.h"
#include "engine/UI/slider.h"
#include "engine/gameObject.h"

class Game;

class Player : public GameObject {
public:
<<<<<<< HEAD
	Player();
=======
  Player();
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void initialize(const vector2Df &position, const Scene &scene) override;
  void update(Scene &scene, const float deltaTime) override;

protected:
<<<<<<< HEAD
	SETOBJECTTEXTURE("player.png");
=======
  SETOBJECTTEXTURE("player.png");
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void onCollision(const GameObject &other) override;

private:
<<<<<<< HEAD
	inline void pointToMouse(const Scene& scene);
	inline void shoot(Scene& scene) const;
=======
  inline void pointToMouse(const Scene &scene);
  inline void shoot(Scene &scene) const;
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  bool moveLeft, moveRight, moveUp, moveDown;
  vector2Df moveDir;
  constexpr static const float moveSpeed = 300;

  // Healthbar
  UI::Background healthbarBG;
  UI::Slider *healthbarSlider;

  float health = 100.0f;
  void takeDamage(const float damage);
};
