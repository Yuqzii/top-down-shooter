#pragma once

#include "engine/collision.h"

#ifdef DEBUG_GIZMO
#include <memory>
class GameObject;
#endif

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

#ifdef DEBUG_GIZMO
	std::unique_ptr<GameObject> fakeObject;
#endif
};
