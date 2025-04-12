#pragma once

#include "enemies/enemy.h"

class SpiderEnemy : public Enemy {
public:
<<<<<<< HEAD
	SpiderEnemy(const float startHealth = 100.0f, const float damage = 10.0f,
				const float moveSpeed = 300.0f, const float maxSteer = 650.0f,
				const float steerMult = 2.0f, const float slowingRadius = 300.0f);
=======
  SpiderEnemy(const float startHealth = 100.0f, const float damage = 10.0f,
              const float moveSpeed = 300.0f, const float maxSteer = 650.0f,
              const float steerMult = 2.0f, const float slowingRadius = 300.0f);
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void update(Scene &scene, const float deltaTime) override;

protected:
<<<<<<< HEAD
	SETOBJECTTEXTURE("spider-sheet.png");
=======
  SETOBJECTTEXTURE("spider-sheet.png");
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  const std::vector<AnimationData> &getAnimationData() const override {
    static const std::vector<AnimationData> data = {
        {4, 20}, // Crawl
        {6, 15}, // Attack
    };
    return data;
  };

  const std::vector<AnimationEvent> &getAnimationEvents() const override {
    return animationEvents;
  }

private:
<<<<<<< HEAD
	std::vector<AnimationEvent> animationEvents;
=======
  std::vector<AnimationEvent> animationEvents;
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void setState(const EnemyStates newState) override;

  void avoidOtherEnemies(const float strength = 0.7f);

  // Called at the appropriate time during the attack animation
  void attack(Scene &scene);
  void attackRangeCheck();
  constexpr static const float repositionTime = 1.0f;
  float repositionTimer = repositionTime;
};
