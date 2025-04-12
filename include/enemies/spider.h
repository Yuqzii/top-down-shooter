#pragma once

#include "enemies/enemy.h"

class SpiderEnemy : public Enemy {
public:
	SpiderEnemy(const float startHealth = 100.0f, const float damage = 10.0f,
			const float moveSpeed = 300.0f, const float maxSteer = 650.0f,
			const float steerMult = 2.0f, const float slowingRadius = 375.0f);

	void update(Scene& scene, const float deltaTime) override;

protected:
	SETOBJECTTEXTURE("spider-sheet.png");

	const std::vector<AnimationData>& getAnimationData() const override {
		static const std::vector<AnimationData> data = {
			{ 4, 20 }, // Crawl
			{ 5, 15 }, // Attack
		};
		return data;
	};

	const std::vector<AnimationEvent>& getAnimationEvents() const override {
		return animationEvents;
	}

private:
	std::vector<AnimationEvent> animationEvents;

	void setState(const EnemyStates newState) override;

	void avoidOtherEnemies(const float strength = 0.7f);

	// Called at the appropriate time during the attack animation
	void attack(Scene& scene);
	void attackRangeCheck();
	constexpr static const float repositionTime = 2.5f;
	float repositionTimer = repositionTime;
};
