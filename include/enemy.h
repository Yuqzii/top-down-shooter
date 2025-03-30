#pragma once

#include "game/UI/background.h"
#include "game/UI/slider.h"
#include "game/gameObject.h"

class Game;

enum class EnemyStates {
	PURSUIT = 0,
	EVADE,
};

class Enemy : public GameObject {
public:
	Enemy(const float& startHealth = 100, const float& moveSpeed = 200,
	   const float& maxSteer = 0.75f, const float& slowingRadius = 200);

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

	// Steering behaviors
	vector2Df seek(const vector2Df& target) const;
	vector2Df flee(const vector2Df& target) const;
	vector2Df pursuit(const GameObject* target, const float& predictionMultiplier = 1.0f) const;
	vector2Df evade(const GameObject* target, const float& predictionMultiplier = 1.0f) const;

	EnemyStates state;

private:
	const float moveSpeed;
	const float maxSteer;
	const float startHealth;
	const float slowingRadius; // Larger for more gradual stop
	float health;

	void die();

	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;
};
