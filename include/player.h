#pragma once

#include "game/gameObject.h"

class Player : public GameObject {
public:
	void update(Game* game, const double& deltaTime) override;

	void shoot(Game* game) const;
private:
	void pointToMouse(Game* game);

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	constexpr static const float moveSpeed = 200;
};
