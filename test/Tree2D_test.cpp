#include <gtest/gtest.h>
#include "game/2DTree.h"

TEST(Tree2DTest, Subtest_1) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(3, 6), vector2Df(17, 15), vector2Df(13, 15), vector2Df(6, 12),
		vector2Df(9, 1), vector2Df(2, 7), vector2Df(10, 19)
	};

	tree.initializeWithList(points);

	std::array<vector2Df, 7> testPoints = {
		vector2Df(10, 10), vector2Df(8, 2), vector2Df(4, 7), vector2Df(16, 16),
		vector2Df(12, 14), vector2Df(0, 5), vector2Df(11, 18)
	};
	std::array<vector2Df, 7> expected = {
		vector2Df(4, 12), vector2Df(9, 1), vector2Df(3, 6), vector2Df(17, 15),
		vector2Df(13, 15), vector2Df(2, 7), vector2Df(10, 19)
	};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestPoint(testPoints[i]);
		EXPECT_TRUE(result == expected[i]) <<
			"Expected: " << expected[i] << " Found: " << result << std::endl;
	}
}
