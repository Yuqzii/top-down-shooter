#pragma once

#include "engine/scene.h"

class MockScene : public Scene {
public:
	MockScene(Game& game) : Scene{game} {}

	int objCount() const { return getGameObjects().size(); }
	int delCount() const {
		int res = 0;
		for (auto& obj : getGameObjects()) {
			if (obj->deleteObject) ++res;
		}
		return res;
	}
};
