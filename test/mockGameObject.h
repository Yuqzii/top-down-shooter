#pragma once
#include "game/gameObject.h"

class MockGameObject : public GameObject {
public:
	MockGameObject(const vector2Df& pos) {
		pivotPosition = pos;
	}
};
