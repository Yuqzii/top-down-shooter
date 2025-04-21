#pragma once

#include "engine/UI/background.h"
#include "engine/UI/slider.h"
#include "engine/gameObject.h"

class Game;
class CombatScene;

enum class EnemyStates {
	PURSUIT = 0,
	EVADE,
	ATTACK,
	REPOSITION,
};

class Enemy : public GameObject {
public:
	Enemy(const float startHealth = 100.0f, const float damage = 10.0f,
		  const float moveSpeed = 300.0f, const float maxSteer = 650.0f,
		  const float steerMult = 2.0f, const float slowingRadius = 100.0f);

	virtual void initialize(const vector2Df& position, const Scene& scene) override;
	virtual void update(Scene& scene, const float deltaTime) override;

	virtual void onCollision(const Collision::Event& event, Scene& scene) override;

	const float damage;

protected:
	CircleCollider& circleCollider;

	// Steering behaviors
	vector2Df steering;
	vector2Df seek(const vector2Df& target) const;
	vector2Df flee(const vector2Df& target) const;
	vector2Df pursuit(const GameObject& target, const float& predictionMultiplier = 1.0f) const;
	vector2Df evade(const GameObject& target, const float& predictionMultiplier = 1.0f) const;

	void avoidTerrain(const float strength, const float avoidDist);

	virtual void setState(const EnemyStates newState) { state = newState; }
	EnemyStates getState() const { return state; }

	bool isMoving;

	const CombatScene* combatScene;

	std::function<void(SDL_Renderer*)> debugRender() const override;

private:
	const float moveSpeed;
	const float maxSteer;
	const float steerStrength;
	const float startHealth;
	const float slowingRadius;	// Larger for more gradual stop
	float health;

	EnemyStates state;

	void takeDamage(const float damage);
	void die();

	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;
};

class EnemyAttackPoint : public GameObject {
public:
	EnemyAttackPoint();

	void initializeParent(const Enemy* parent);
	const Enemy* parent;

private:
	SETOBJECTTEXTURE("empty.png");
};
