#pragma once

#include "engine/gameObject.h"

class Game;
class GunData;

class Bullet : public GameObject {
public:
	Bullet();

	void initialize(const vector2Df& startPosition, const Scene& scene) override;
	void update(Scene& scene, const float deltaTime) override;

	void onCollision(const Collision::Event& event, Scene& scene) override;

	/*
	 * @abstract	Initializes the bullet with a direction, and its corresponding GunData object.
	 *				If this function is not called the bullet will not work,
	 *				and likely results in errors.
	 * @param		direction	The direction the bullet will travel in.
	 * @param		rotation	The rotation of the bullet. (Used because rotation is already
	 *							calculated for the player, avoids recomputation).
	 * @param		gunData		Data object for the gun this bullet was fired from.
	 */
	void initializeBullet(const vector2Df& direction, const float rotation, const GunData& gunData);

	const GunData& getData() const { return *data; }

protected:
	SETOBJECTTEXTURE("bullet.png");

private:
	vector2Df direction;
	LineCollider* lineCollider;

	const GunData* data;

	constexpr static const float startTime = 4;
	float timeLeft = startTime;
};
