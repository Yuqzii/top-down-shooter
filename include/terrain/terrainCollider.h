#pragma once

#include "engine/gameObject.h"

class Chunk;

class TerrainCollider {
public:
	TerrainCollider(Vec2&& position, Vec2&& start, Vec2&& end, Chunk& chunk);

	// ONLY USED FOR DEBUG_GIZMO
	void update(Scene& scene, const float deltaTime);

	void onCollision(const Collision::Event& event, Scene& scene);

private:
	Chunk& chunk;

	LineCollider collider;
	Vec2 position;

	// testing, REMOVE
	Vec2 normal;
};
