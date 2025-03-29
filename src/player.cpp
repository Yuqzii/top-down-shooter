#include <cmath>
#include "SDL2/SDL_scancode.h"
#include "game/game.h"
#include "player.h"
#include "bullet.h"
#include "game/gameObject.h"

Player::Player() : healthbarBG(vector2Df(20, 0), vector2Df(250, 30), SDL_Color{ 255, 0, 0, 255 }) {
	pivotOffset.y = 20;

	healthbarSlider = new UI::Slider(SDL_Color{ 0, 255, 0, 255 }, &healthbarBG);
}

void Player::initialize(const vector2Df& position, Game* game) {
	GameObject::initialize(position, game); // Call base initialize
	
	circleCollider.radius = 40; // Change collider size
}

// Do player specific processing here
void Player::update(Game* game, const double& deltaTime) {
	
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

	moveDir = moveDir.normalized(); // Normalize vector so that diagonal movement is not faster
	velocity = moveDir * moveSpeed; // Update velocity

	GameObject::update(game, deltaTime); // Call base GameObject update (Updates position)
	
	pointToMouse(game);

	if (game->mouseInput[SDL_BUTTON_LEFT]) {
		shoot(game);
	}

	healthbarBG.update();

	int windowHeight;
	SDL_GetWindowSizeInPixels(game->getWindow(), NULL, &windowHeight);
	// Make space from bottom of screen to healthbar same as side of screen to healthbar
	healthbarBG.localPosition.y = windowHeight - healthbarBG.localSize.y -
		healthbarBG.localPosition.x;
	healthbarBG.calculatePosition();
	
	// Tell UIManager to render healthbar
	game->getUIManager()->addRenderCall(healthbarBG.getRenderFunction());

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
