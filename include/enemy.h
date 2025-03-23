#pragma once

#include "game/gameObject.h"

class Game;

class Enemy : public GameObject {
public:
	void initialize(const std::string& textureSheet,
					const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

private:
	constexpr static const float moveSpeed = 150;
};
