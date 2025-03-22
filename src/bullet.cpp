#include "bullet.h"
#include <iostream>

float Bullet::speed = 350;

Bullet::Bullet(const vector2Df direction) {
	this->direction = direction;
	rotation = direction.toDegrees();
}

void Bullet::update(Game* game, const double& deltaTime) {
	position.x += speed * direction.x * deltaTime;
	position.y += speed * direction.y * deltaTime;

	std::cout << "Updating bullet at position " << position.x << "   " << position.y << std::endl;
}
