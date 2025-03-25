#pragma once

#include "game/gameObject.h"

class Game;

class Enemy : public GameObject {
public:
	Enemy();

	void initialize(const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

	void takeDamage(const float& damage);

protected:
	SETOBJECTTEXTURE("spider-sheet.png");

	const std::vector<AnimationData>& getAnimationData() const override {
		static const std::vector<AnimationData> data = {
			{ 4, 15 },
		};
		return data;
	};

private:
	constexpr static const float moveSpeed = 150;
	constexpr static const float startHealth = 100;
	float health;

	void die();
};
