#pragma once

#include "enemies/enemy.h"

class SpiderEnemy : public Enemy {
public:
	SpiderEnemy(const float startHealth = 100.0f, const float damage = 10.0f,
			const float moveSpeed = 300.0f, const float maxSteer = 650.0f,
			const float steerMult = 2.0f, const float slowingRadius = 600.0f);

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
		static const std::vector<AnimationEvent> events = {
			{ 1, 4, attackEvent },
		};
		return events;
	};

private:
	void attack(Scene& scene);
	const std::function<void(Scene&)> attackEvent = [this](Scene& scene){ attack(scene); };
};
