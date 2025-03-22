#include <cmath>
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

inline void Player::pointToMouse(Game* game) {
	vector2Df midPos = midPosition();
	vector2Df direction(game->mousePos.x - midPos.x, game->mousePos.y - midPos.y);
	rotation = direction.toDegrees() + 90;
}

inline void Player::shoot(Game* game) const {
	// TODO: Swap "Player.png" for actual bullet sprite
	// Instantiate bullet
	Bullet* bullet = game->instantiate<Bullet>("Player.png", position);
	float radians = (rotation - 90) * M_PI / 180; // Convert player rotation to radians
	// Initialize bullet with correct rotation
	bullet->initializeDirection(vector2Df((float)std::cos(radians),
								(float)std::sin(radians)), rotation);
}
