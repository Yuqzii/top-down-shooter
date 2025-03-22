#include <cmath>
#include <iostream>
#include "SDL2/SDL_scancode.h"
#include "bullet.h"
#include "player.h"

// Do player specific processing here
void Player::update(Game* game, const double& deltaTime) {
	GameObject::update(game, deltaTime); // Call base GameObject update
	
	// Get input
	moveLeft = game->input[SDL_SCANCODE_A];
	moveRight = game->input[SDL_SCANCODE_D];
	moveUp = game->input[SDL_SCANCODE_W];
	moveDown = game->input[SDL_SCANCODE_S];

	// Update movement direction according to input
	moveDir = { 0, 0}; // Reset movement direction
	if (moveLeft) {
		moveDir.x = -1;
	}
	else if (moveRight) {
		moveDir.x = 1;
	}
	if (moveUp) {
		moveDir.y = -1;
	}
	else if (moveDown) {
		moveDir.y = 1;
	}

	moveDir.normalize(); // Normalize vector so that diagonal movement is not faster
	// Apply movement
	position.x += moveDir.x * moveSpeed * deltaTime;
	position.y += moveDir.y * moveSpeed * deltaTime;

	pointToMouse(game);

	if (game->mouseInput[SDL_BUTTON_LEFT]) {
		shoot(game);
	}
}

void Player::pointToMouse(Game* game) {
	vector2Df midPos = midPosition();
	vector2Df direction(game->mousePos.x - midPos.x, game->mousePos.y - midPos.y);
	rotation = direction.toDegrees() + 90;
}

void Player::shoot(Game* game) const {
	std::cout << "Shoot!\n";
	
	vector2Df midPos = midPosition();
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(
		vector2Df(game->mousePos.x - midPos.x, game->mousePos.y - midPos.y));
	bullet->initialize("Player.png", position, game);
}
