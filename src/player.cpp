#include "player.h"

#include <cmath>

#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_scancode.h"
#include "bullet.h"
#include "enemies/enemy.h"
#include "engine/game.h"
#include "engine/gameObject.h"
#include "engine/scene.h"
#include "terrain/terrainCollider.h"

Player::Player()
	: healthbarBG{vector2Df(20, 0), vector2Df(250, 30), SDL_Color{255, 0, 0, 255}},
	  currentGun{std::make_unique<GunData>("Sick ass gun", 20, 2000, true, 0.1f)},
	  timeSinceShot{0.0f},
	  GameObject{std::make_unique<CircleCollider>(std::move(Collision::Circle{40.0f}), 500.0f, this)},
	  circleCollider{static_cast<CircleCollider&>(*collider)}{
	  
	pivotOffset.y = 20;

	healthbarSlider = new UI::Slider(SDL_Color{0, 255, 0, 255}, &healthbarBG);
}

void Player::initialize(const vector2Df& position, const Scene& scene) {
	GameObject::initialize(position, scene);  // Call base initialize
}

// Do player specific processing here
void Player::update(Scene& scene, const float deltaTime) {
	// Get input
	moveLeft = scene.getGame().getInput()[SDL_SCANCODE_A];
	moveRight = scene.getGame().getInput()[SDL_SCANCODE_D];
	moveUp = scene.getGame().getInput()[SDL_SCANCODE_W];
	moveDown = scene.getGame().getInput()[SDL_SCANCODE_S];

	// Update movement direction according to input
	moveDir = {0, 0};  // Reset movement direction
	if (moveLeft) {
		moveDir.x = -1;
	} else if (moveRight) {
		moveDir.x = 1;
	}
	if (moveUp) {
		moveDir.y = -1;
	} else if (moveDown) {
		moveDir.y = 1;
	}

	moveDir = moveDir.normalized();	 // Normalize vector so that diagonal movement is not faster
	velocity = moveDir * moveSpeed;	 // Update velocity

	GameObject::update(scene, deltaTime);  // Call base GameObject update (Updates position)
	pointToMouse(scene);

	circleCollider.circle.position = position + getDirection() * 10.0f; // Update collider position

	timeSinceShot += deltaTime;
	const bool enoughTimePassed = timeSinceShot >= currentGun->timeBetweenShots;
	if (currentGun->isAuto) {
		if (scene.getGame().getMouseInput()[SDL_BUTTON_LEFT] && enoughTimePassed) shoot(scene);
	} else {
		if (scene.getGame().getOnMouseDown()[SDL_BUTTON_LEFT] && enoughTimePassed) shoot(scene);
	}

	healthbarBG.update();

	int windowHeight;
	SDL_GetWindowSizeInPixels(scene.getGame().getWindow(), NULL, &windowHeight);
	// Make space from bottom of screen to healthbar same as side of screen to healthbar
	healthbarBG.localPosition.y =
		windowHeight - healthbarBG.localSize.y - healthbarBG.localPosition.x;
	healthbarBG.calculatePosition();

	// Tell UIManager to render healthbar
	scene.getGame().getRenderManager().addRenderCall(healthbarBG.getRenderFunction(), this);
}

// Points player towards the mouse
inline void Player::pointToMouse(const Scene& scene) {
	vector2Df direction(scene.getGame().getMousePos().x - position.x,
						scene.getGame().getMousePos().y - position.y);
	rotation = direction.toDegrees() + 90;
}

void Player::shoot(Scene& scene) {
	vector2Df direction(rotation);
	// Instantiate bullet
	constexpr float distMultiplier = 75;  // How much further than player center should bullet spawn
	Bullet& bullet = scene.instantiate<Bullet>(vector2Df(
		position.x + direction.x * distMultiplier, position.y + direction.y * distMultiplier));
	// Initialize bullet with correct rotation
	bullet.initializeBullet(direction, rotation, *currentGun);

	timeSinceShot = 0.0f;
}

void Player::onCollision(const Collision::Event& event) {
	const EnemyAttackPoint* enemyAttackPoint =
		dynamic_cast<const EnemyAttackPoint*>(event.other->getParent());
	if (enemyAttackPoint) {
		takeDamage(enemyAttackPoint->parent->damage);
		return;
	} 
		
	const TerrainCollider* terrainCollider =
		dynamic_cast<const TerrainCollider*>(event.other->getParent());
	if (terrainCollider) {
		position += Collision::resolveStaticLine(event, position);
		return;
	}
}

void Player::takeDamage(const float damage) {
	health -= damage;
	healthbarSlider->setValue(health);
}
