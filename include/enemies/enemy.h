#pragma once

#include "engine/UI/background.h"
#include "engine/UI/slider.h"
#include "engine/gameObject.h"

class Game;
class CombatScene;

enum class EnemyStates {
	PURSUIT = 0,
	EVADE,
};

class Enemy : public GameObject {
public:
	Enemy(const float& startHealth = 100.0f, const float& moveSpeed = 300.0f,
	   const float& maxSteer = 650.0f, const float& steerMult = 2.0f,
	   const float& slowingRadius = 100.0f);

	void initialize(const vector2Df& position, const Scene& scene) override;
	void update(Scene& scene, const float deltaTime) override;

	void onCollision(const GameObject* other) override;

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
	vector2Df steering;
	vector2Df seek(const vector2Df& target) const;
	vector2Df flee(const vector2Df& target) const;
	vector2Df pursuit(const GameObject& target, const float& predictionMultiplier = 1.0f) const;
	vector2Df evade(const GameObject& target, const float& predictionMultiplier = 1.0f) const;

	EnemyStates state;

	std::function<void(SDL_Renderer*)> debugRender() const override;

private:
	const CombatScene* combatScene;

	const float moveSpeed;
	const float maxSteer;
	const float steerStrength;
	const float startHealth;
	const float slowingRadius; // Larger for more gradual stop
	float health;

	void die();

	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;
};
