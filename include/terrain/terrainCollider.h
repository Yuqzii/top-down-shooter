#pragma once

#include <memory>

#include "engine/collision.h"

class Chunk;
class GameObject;

class TerrainCollider : public LineCollider {
public:
	TerrainCollider(Vec2&& position, Vec2&& start, Vec2&& end, Chunk& chunk);

	// Delete copy
	TerrainCollider(const TerrainCollider&) = delete;
	TerrainCollider& operator=(const TerrainCollider&) = delete;

	TerrainCollider(TerrainCollider&&) = default;

	void update(Scene& scene);

	void onCollision(const Collision::Event& event, Scene& scene) override;

	std::string_view getTag() const override { return "Terrain"; }

private:
	Chunk& chunk;

	Vec2 position;

	std::unique_ptr<GameObject> fakeObject;
};
