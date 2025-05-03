#pragma once

#include "engine/UI/background.h"
#include "engine/UI/slider.h"
#include "engine/gameObject.h"
#include "gunData.h"

class Game;

class Player : public GameObject {
public:
	Player();

	void initialize(const Scene& scene, const Vec2& startPos);
	void update(Scene& scene, const float deltaTime) override;

private:
	SETOBJECTTEXTURE("player.bmp");

	inline void pointToMouse(const Scene& scene);
	inline void shoot(Scene& scene);

	CircleCollider& circleCollider;
	void onCollision(const Collision::Event& event, Scene& scene) override;

	bool moveLeft, moveRight, moveUp, moveDown;
	Vec2 moveDir;
	constexpr static const float moveSpeed = 300;

	// Healthbar
	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;

	float health = 100.0f;
	void takeDamage(const float damage);

	std::shared_ptr<GunData> currentGun;
	float timeSinceShot;
};
