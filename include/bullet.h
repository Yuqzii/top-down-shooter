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

	const float startTime = 10;
	float timeLeft = startTime;
};
