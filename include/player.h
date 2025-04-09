#pragma once

#include "engine/UI/background.h"
#include "engine/UI/slider.h"
#include "engine/gameObject.h"

class Game;

class Player : public GameObject {
public:
	Player();

	void initialize(const vector2Df& position, const Scene& scene) override;
	void update(Scene& scene, const float deltaTime) override;

protected:
	SETOBJECTTEXTURE("player.png");

private:
	inline void pointToMouse(const Scene& scene);
	inline void shoot(Scene& scene) const;

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	constexpr static const float moveSpeed = 300;

	// Healthbar
	UI::Background healthbarBG;
	UI::Slider* healthbarSlider;
};
