#include <cmath>
#include "SDL2/SDL_scancode.h"
#include "engine/gameObject.h"
#include "engine/scene.h"
#include "engine/game.h"
#include "player.h"
#include "bullet.h"

Player::Player() : healthbarBG(vector2Df(20, 0), vector2Df(250, 30), SDL_Color{ 255, 0, 0, 255 }) {
	pivotOffset.y = 20;

	healthbarSlider = new UI::Slider(SDL_Color{ 0, 255, 0, 255 }, &healthbarBG);
}

void Player::initialize(const vector2Df& position, const Scene& scene) {
	GameObject::initialize(position, scene); // Call base initialize
	
	circleCollider.radius = 40; // Change collider size
}

// Do player specific processing here
void Player::update(Scene& scene, const float deltaTime) {
	
	// Get input
	moveLeft = scene.getGame().getInput()[SDL_SCANCODE_A];
	moveRight = scene.getGame().getInput()[SDL_SCANCODE_D];
	moveUp = scene.getGame().getInput()[SDL_SCANCODE_W];
	moveDown = scene.getGame().getInput()[SDL_SCANCODE_S];

	// Update movement direction according to input
	moveDir = { 0, 0 }; // Reset movement direction
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

	GameObject::update(scene, deltaTime); // Call base GameObject update (Updates position)
	
	pointToMouse(scene);

	if (scene.getGame().getMouseInput()[SDL_BUTTON_LEFT]) {
		shoot(scene);
	}

	healthbarBG.update();

	int windowHeight;
	SDL_GetWindowSizeInPixels(scene.getGame().getWindow(), NULL, &windowHeight);
	// Make space from bottom of screen to healthbar same as side of screen to healthbar
	healthbarBG.localPosition.y = windowHeight - healthbarBG.localSize.y -
		healthbarBG.localPosition.x;
	healthbarBG.calculatePosition();
	
	// Tell UIManager to render healthbar
	scene.getGame().getRenderManager().addRenderCall(healthbarBG.getRenderFunction(), this);

}

// Points player towards the mouse
inline void Player::pointToMouse(const Scene& scene) {
	vector2Df direction(scene.getGame().getMousePos().x - pivotPosition.x,
			scene.getGame().getMousePos().y - pivotPosition.y);
	rotation = direction.toDegrees() + 90;
}

inline void Player::shoot(Scene& scene) const {
	vector2Df direction(rotation);
	// Instantiate bullet
	constexpr float distMultiplier = 50; // How much further than player center should bullet spawn
	Bullet& bullet = scene.instantiate<Bullet>(
					vector2Df(midPosition.x + direction.x * distMultiplier,
					midPosition.y + direction.y * distMultiplier));
	// Initialize bullet with correct rotation
	bullet.initializeDirection(direction, rotation);
}
