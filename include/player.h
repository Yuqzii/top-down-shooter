#pragma once

#include "game/gameObject.h"

class Game;

class Player : public GameObject {
public:
	Player();

	void initialize(const std::string& textureSheet,
					const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

private:
	inline void pointToMouse(Game* game);
	inline void shoot(Game* game) const;

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	constexpr static const float moveSpeed = 300;
};
