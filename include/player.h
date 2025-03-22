#pragma once

#include "game/gameObject.h"

class Player : public GameObject {
public:
	void update(Game* game, const double& deltaTime) override;

private:
	inline void pointToMouse(Game* game);
	inline void shoot(Game* game) const;

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	constexpr static const float moveSpeed = 300;
};
