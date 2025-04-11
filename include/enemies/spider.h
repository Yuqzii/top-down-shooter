#pragma once

#include "enemies/enemy.h"

class SpiderEnemy : public Enemy {
public:
	SpiderEnemy(const float& startHealth = 100.0f, const float& moveSpeed = 300.0f,
	   const float& maxSteer = 650.0f, const float& steerMult = 2.0f,
	   const float& slowingRadius = 100.0f);

	void update(Scene& scene, const float deltaTime) override;

protected:
	SETOBJECTTEXTURE("spider-sheet.png");

	const std::vector<AnimationData>& getAnimationData() const override {
		static const std::vector<AnimationData> data = {
			{ 4, 20 }, // Crawl animation
		};
		return data;
	};
};
