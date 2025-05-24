#pragma once

#include "engine/vector2D.h"

class Camera {
public:
	Vec2 getPos() const { return pos; }
	void setPos(const Vec2& newPos) { pos = newPos; }

private:
	Vec2 pos;
};
