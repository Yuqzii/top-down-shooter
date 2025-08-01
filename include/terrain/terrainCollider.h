#pragma once

#include "engine/gameObject.h"

class Chunk;

class TerrainCollider : public GameObject {
public:
	TerrainCollider();

	void initialize(const Scene& scene, const Vec2& position, const Vec2& start, const Vec2& end,
					Chunk* chunk);

	// ONLY USED FOR DEBUG_GIZMO
	void update(Scene& scene, const float deltaTime) override;

	void onCollision(const Collision::Event& event, Scene& scene) override;

private:
	SETOBJECTTEXTURE("empty.bmp");

	Chunk* chunk;

	// testing, REMOVE
	Vec2 normal;
};
