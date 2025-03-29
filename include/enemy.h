#pragma once

#include "game/UI/background.h"
#include "game/UI/slider.h"
#include "game/gameObject.h"

class Game;

enum class EnemyStates {
	PURSUIT = 0,
	EVADE
};

class Enemy : public GameObject {
public:
	Enemy(const float& startHealth = 100, const float& moveSpeed = 200, const float& maxSteer = 0.5,
	   const float& slowingRadius = 200);

	void initialize(const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

	void takeDamage(const float& damage);

protected:
	SETOBJECTTEXTURE("spider-sheet.png");


	const std::vector<AnimationData>& getAnimationData() const override {
		static const std::vector<AnimationData> data = {
			{ 4, 20 }, // Crawl animation
		};
		return data;
	};

private:
	const float moveSpeed = 200;
	const float maxSteer = 0.5;
	const float startHealth = 100;
	const float slowingRadius = 200; // Larger for more gradual stop
	float health;

	void die();

	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;
};
