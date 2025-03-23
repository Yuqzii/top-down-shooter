#pragma once

#include "game/gameObject.h"

class Game;

class Enemy : public GameObject {
public:
	void initialize(const std::string& textureSheet,
					const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

	void takeDamage(const float& damage);

private:
	constexpr static const float moveSpeed = 150;
	constexpr static const float startHealth = 100;
	float health;

	void die();
};
