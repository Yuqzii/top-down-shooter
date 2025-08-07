#pragma once

#include "engine/collision.h"

class Chunk;

class TerrainCollider : public LineCollider {
public:
	TerrainCollider(Vec2&& position, Vec2&& start, Vec2&& end, Chunk& chunk);

	void update(Scene& scene);

	void onCollision(const Collision::Event& event, Scene& scene) override;

	std::string_view getTag() const override { return "Terrain"; }

private:
	Chunk& chunk;

	Vec2 position;

	// testing, REMOVE
	Vec2 normal;
};
