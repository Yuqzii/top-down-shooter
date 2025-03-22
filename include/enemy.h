#pragma once

#include "game/gameObject.h"

class Game;

class Enemy : public GameObject {
public:
	void update(Game* game, const double& deltaTime) override;

private:
	constexpr static const float moveSpeed = 150;
};
