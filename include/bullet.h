#pragma once

#include "engine/gameObject.h"

class Game;
struct GunData;

class Bullet : public GameObject {
public:
	Bullet();

	void initialize(const Scene& scene, const Vec2& startPos, const Vec2& direction,
				 const float rotation, const std::shared_ptr<GunData>& gunData);
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
	const GunData& getData() const { return *data; }

protected:
	SETOBJECTTEXTURE("bullet.bmp");

private:
	Vec2 direction;
	LineCollider* lineCollider;

	std::shared_ptr<GunData> data;

	constexpr static const float startTime = 4;
	float timeLeft = startTime;
};
