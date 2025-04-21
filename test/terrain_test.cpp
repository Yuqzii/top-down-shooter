#include <gtest/gtest.h>

#include "mockScene.h"
#include "terrain/terrainManager.h"

TEST(Terrain, CollisionGeneration) {
	const std::vector<std::vector<char>> terrainMap{
		std::vector<char>{1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
		std::vector<char>{1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0},
		std::vector<char>{1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0},
		std::vector<char>{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
		std::vector<char>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		std::vector<char>{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
	};

	Game game{"", 1, 1};
	MockScene scene{game};
	TerrainManager manager{terrainMap, SDL_Color{1, 1, 1, 1}, scene};
	manager.updateCollisions();

	constexpr int expected = 44;
	EXPECT_TRUE(scene.objCount() == expected)
		<< "Expected " << expected << " colliders, found " << scene.objCount();
}
