#pragma once

#include "game/gameObject.h"

class Game;

class Bullet : public GameObject {
public:
	void update(Game* game, const double& deltaTime) override;

	void initializeDirection(const vector2Df direction, const float rotation);

private:
	vector2Df direction;
	constexpr static const float speed = 1000;

	constexpr static const float startTime = 4;
	float timeLeft = startTime;
};
