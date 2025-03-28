#pragma once

#include "game/UI/background.h"
#include "game/gameObject.h"

class Game;

class Player : public GameObject {
public:
	Player();

	void initialize(const vector2Df& position, Game* game) override;
	void update(Game* game, const double& deltaTime) override;

protected:
	SETOBJECTTEXTURE("player.png");

private:
	inline void pointToMouse(Game* game);
	inline void shoot(Game* game) const;

	bool moveLeft, moveRight, moveUp, moveDown;
	vector2Df moveDir;
	constexpr static const float moveSpeed = 300;

	// Healthbar
	UI::Background healthbar;
};
