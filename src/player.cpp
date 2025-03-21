#include <cmath>
#include "SDL2/SDL_scancode.h"
#include "player.h"

float Player::moveSpeed = 200;

// Do player specific processing here
void Player::update(Game* game, double deltaTime) {
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

	// Ensure vector is normalized, so that diagonal movement is not faster
	float moveDirLength = std::sqrt(std::pow(moveDir.x, 2.0) + std::pow(moveDir.y, 2.0));
	if (moveDirLength > 0) {
		moveDir.x /= moveDirLength;
		moveDir.y /= moveDirLength;
	}

	// Apply movement
	position.x += moveDir.x * moveSpeed * deltaTime;
	position.y += moveDir.y * moveSpeed * deltaTime;
}
