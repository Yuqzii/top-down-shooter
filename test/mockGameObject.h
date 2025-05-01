#pragma once
#include "engine/gameObject.h"

class MockGameObject : public GameObject {
public:
	MockGameObject(const Vec2& pos) { position = pos; }
};
