#pragma once

#include "engine/gameObject.h"

class TerrainManager;

class TerrainCollider : public GameObject {
public:
	TerrainCollider();

	void initializeCollider(const vector2Df& start, const vector2Df& end, TerrainManager& manager);

	// ONLY USED FOR DEBUG_GIZMO
	void update(Scene& scene, const float deltaTime) override;

	void onCollision(const Collision::Event& event) override;

private:
	SETOBJECTTEXTURE("empty.png");

	TerrainManager* manager;

	// testing, REMOVE
	vector2Df normal;
};
