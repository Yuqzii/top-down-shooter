#pragma once

#include "engine/scene.h"

class MockScene : public Scene {
public:
	MockScene(Game& game) : Scene{game} {}
	int objCount() const { return getGameObjects().size(); }
};
