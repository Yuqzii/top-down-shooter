#include <gtest/gtest.h>
#include "game/Tree2D.h"

TEST(Tree2DTest, General) {
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
		vector2Df(6, 12), vector2Df(9, 1), vector2Df(3, 6), vector2Df(17, 15),
		vector2Df(13, 15), vector2Df(2, 7), vector2Df(10, 19)
	};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestPoint(testPoints[i]);
		EXPECT_TRUE(result == expected[i]) <<
			"Expected: " << expected[i] << " Found: " << result << std::endl;
	}
}

TEST(Tree2DTest, CheckingWithExistingPoints) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(10, 10), vector2Df(20, 10), vector2Df(10, 5), vector2Df(5, 20),
	};

	tree.initializeWithList(points);
	
	std::array<vector2Df, 2> testPoints = {
		vector2Df(10, 10), vector2Df(10, 5)
	};
	
	std::array expected = {
		vector2Df(10, 5), vector2Df(10, 10)
	};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestPoint(testPoints[i]);
		EXPECT_TRUE(result == expected[i]) <<
			"Expected: " << expected[i] << " Found: " << result << std::endl;
	}
}

TEST(Tree2DTest, SinglePoint) {
	Tree2D tree;

	tree.insert(vector2Df(10, 10));

	std::array<vector2Df, 2> testPoints = {
		vector2Df(10, 10), vector2Df(10, 5)
	};
	
	EXPECT_THROW(tree.findClosestPoint(testPoints[0]), int) <<
		"Expected error 2 thrown";

	vector2Df expected(10, 10);
	const vector2Df result = tree.findClosestPoint(testPoints[1]);
	EXPECT_TRUE(result == expected) <<
		"Expected: " << expected << " Found: " << result << std::endl;
}

TEST(Tree2DTest, DuplicatePoints) {
	Tree2D tree;

	tree.insert(vector2Df(10, 10));
	tree.insert(vector2Df(10, 10));
	tree.insert(vector2Df(10, 10));

	EXPECT_THROW(tree.findClosestPoint(vector2Df(10, 10)), int) <<
		"Expected error 2 thrown." << std::endl;

	vector2Df result = tree.findClosestPoint(vector2Df(5, 2));
	EXPECT_TRUE(result == vector2Df(10, 10)) <<
		"Expected: " << vector2Df(10, 10) << " Found: " << result << std::endl;

	result = tree.findClosestPoint(vector2Df(1000000000, 1000000000));
	EXPECT_TRUE(result == vector2Df(10, 10)) <<
		"Expected: " << vector2Df(10, 10) << " Found: " << result << std::endl;
}

TEST(Tree2DTest, MultiplePointQuery) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(3, 6), vector2Df(17, 15), vector2Df(13, 15), vector2Df(6, 12),
		vector2Df(9, 2), vector2Df(2, 7), vector2Df(10, 19)
	};

	tree.initializeWithList(points);

	std::array<vector2Df, 3> testPoints = {
		vector2Df(2, 5), vector2Df(12, 12), vector2Df(25, 5)
	};

	std::array<std::vector<vector2Df>, 3> expected = {
		std::vector { vector2Df(3, 6), vector2Df(2, 7), vector2Df(9, 2) },
		std::vector { vector2Df(13, 15), vector2Df(17, 15), vector2Df(6, 12), vector2Df(10, 19) },
		std::vector { vector2Df(17, 15), vector2Df(13, 15), vector2Df(9, 2), vector2Df(6, 12),
				vector2Df(10, 19), vector2Df(3, 6) }
	};

	for (int i = 0; i < testPoints.size(); i++) {
		std::vector<vector2Df> result = tree.findKClosestPoints(testPoints[i], expected[i].size());
	
		EXPECT_TRUE(result.size() == expected[i].size()) << "Expected size: " << expected[i].size()
				<< " Found size: " << result.size() << ". Subtest number " << i;

		for (int j = 0; j < result.size(); j++) {
			EXPECT_TRUE(result[j] == expected[i][j]) << "Expected: " << expected[i][j] <<
					" Found: " << result[j] << ". Subtest number " << i;
		}


	}
}
