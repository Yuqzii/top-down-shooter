#pragma once

#include "game/gameObject.h"

class Player : public GameObject {
public:
	void update(Game* game, double deltaTime) override;
private:
	void pointToMouse(Game* game);

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	static float moveSpeed;
};
