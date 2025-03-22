#pragma once

#include "game/gameObject.h"

class Bullet : public GameObject {
public:
	Bullet();

	void update(Game* game, const double& deltaTime) override;

	void initializeDirection(const vector2Df direction, const float rotation);

private:
	vector2Df direction;
	constexpr static const float speed = 1000;

	constexpr static const float startTime = 4;
	float timeLeft = startTime;
};
