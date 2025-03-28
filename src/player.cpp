#include <cmath>
#include "SDL2/SDL_scancode.h"
#include "game/game.h"
#include "player.h"
#include "bullet.h"
#include "game/gameObject.h"

Player::Player() : healthbar(vector2Df(), vector2Df(200, 50), SDL_Color{ 0, 255, 0, 255 }) {
	pivotOffset.y = 20;
}

void Player::initialize(const vector2Df& position, Game* game) {
	GameObject::initialize(position, game); // Call base initialize
	
	circleCollider.radius = 40; // Change collider size
}

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

	healthbar.localPosition = midPosition;
	healthbar.calculatePosition();
	healthbar.update();
	// Tell UIManager to render healthbar
	game->getUIManager()->addRenderCall(healthbar.getRenderFunction());
}

// Points player towards the mouse
inline void Player::pointToMouse(Game* game) {
	vector2Df direction(game->mousePos.x - pivotPosition.x, game->mousePos.y - pivotPosition.y);
	rotation = direction.toDegrees() + 90;
}

inline void Player::shoot(Game* game) const {
	vector2Df direction(rotation);
	// Instantiate bullet
	constexpr float distMultiplier = 50; // How much further than player center should bullet spawn
	Bullet* bullet = game->instantiate<Bullet>(
					vector2Df(midPosition.x + direction.x * distMultiplier,
					midPosition.y + direction.y * distMultiplier));
	// Initialize bullet with correct rotation
	bullet->initializeDirection(direction, rotation);
}
