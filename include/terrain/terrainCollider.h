#pragma once

#include "engine/gameObject.h"

class TerrainCollider : public GameObject {
public:
	TerrainCollider();

	void initializeCollider(const vector2Df& start, const vector2Df& end);

	void update(Scene& scene, const float deltaTime) override;

private:
	SETOBJECTTEXTURE("empty.png");

	// testing, REMOVE
	vector2Df normal;
};
