#pragma once

#include "game/gameObject.h"

class Bullet : public GameObject {
public:
	Bullet(const vector2Df direction);
	void update(Game* game, const double& deltaTime) override;

private:
	vector2Df direction;
	static float speed;
};
