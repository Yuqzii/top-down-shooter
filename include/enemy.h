#pragma once

#include "game/gameObject.h"

class Game;

class Enemy : public GameObject {
public:
	void initialize(const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

	void takeDamage(const float& damage);

protected:
	std::string getTextureSheet() const override { return "spider-sheet.png"; };

private:
	constexpr static const float moveSpeed = 150;
	constexpr static const float startHealth = 100;
	float health;

	void die();
};
