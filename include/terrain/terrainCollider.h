#pragma once

#include "engine/gameObject.h"

class TerrainCollider : public GameObject {
public:
	TerrainCollider();

	void initializeCollider(const vector2Df& start, const vector2Df& end);

private:
	SETOBJECTTEXTURE("empty.png");
};
