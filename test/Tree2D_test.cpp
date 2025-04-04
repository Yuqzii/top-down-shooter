#include <gtest/gtest.h>
#include "game/Tree2D.h"
#include "game/gameObject.h"
#include "mockGameObject.h"

std::vector<GameObject*> testInit(const std::vector<vector2Df>& points) {
	std::vector<MockGameObject*> objects;
	objects.reserve(points.size());
	for (const vector2Df& point : points) {
		objects.push_back(new MockGameObject(point));
	}

	return std::vector<GameObject*>(objects.begin(), objects.end());
}

void testCleanup(std::vector<GameObject*>& objects) {
	for (GameObject* obj : objects) {
		delete obj;
	}
}

TEST(Tree2DTest, General) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(3, 6), vector2Df(17, 15), vector2Df(13, 15), vector2Df(6, 12),
		vector2Df(9, 1), vector2Df(2, 7), vector2Df(10, 19)
	};

	auto testData = testInit(points);
	tree.initializeWithList(testData);

	std::array<vector2Df, 7> testPoints = {
		vector2Df(10, 10), vector2Df(8, 2), vector2Df(4, 7), vector2Df(16, 16),
		vector2Df(12, 14), vector2Df(0, 5), vector2Df(11, 18)
	};
	std::array<vector2Df, 7> expected = {
		vector2Df(6, 12), vector2Df(9, 1), vector2Df(3, 6), vector2Df(17, 15),
		vector2Df(13, 15), vector2Df(2, 7), vector2Df(10, 19)
	};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestPoint(testPoints[i])->getPivotPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testData);
}

TEST(Tree2DTest, CheckingWithExistingPoints) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(10, 10), vector2Df(20, 10), vector2Df(10, 5), vector2Df(5, 20),
	};

	auto testData = testInit(points);
	tree.initializeWithList(testData);
	
	std::array<vector2Df, 2> testPoints = {
		vector2Df(10, 10), vector2Df(10, 5)
	};
	
	std::array expected = {
		vector2Df(10, 5), vector2Df(10, 10)
	};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestPoint(testPoints[i])->getPivotPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testData);
}

TEST(Tree2DTest, SinglePoint) {
	Tree2D tree;

	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());

	std::array<vector2Df, 2> testPoints = {
		vector2Df(10, 10), vector2Df(10, 5)
	};
	
	EXPECT_THROW(tree.findClosestPoint(testPoints[0]), int) <<
		"Expected error 2 thrown";

	vector2Df expected(10, 10);
	const vector2Df result = tree.findClosestPoint(testPoints[1])->getPivotPosition();
	EXPECT_TRUE(result == expected) << "Expected: " << expected << " Found: " << result;
}

TEST(Tree2DTest, DuplicatePoints) {
	Tree2D tree;

	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(15, 15)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());

	vector2Df result = tree.findClosestPoint(vector2Df(5, 2))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(10, 10)) <<
			"Expected: " << vector2Df(10, 10) << " Found: " << result;

	result = tree.findClosestPoint(vector2Df(1000000000, 1000000000))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(10, 10)) <<
			"Expected: " << vector2Df(10, 10) << " Found: " << result;

	result = tree.findClosestPoint(vector2Df(10, 10))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(15, 15)) <<
			"Expected: " << vector2Df(15, 15) << " Found: " << result;
}

TEST(Tree2DTest, DuplicateSinglePoints) {
	Tree2D tree;

	tree.insert(std::make_unique<MockGameObject>(vector2Df(5, 3)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(5, 3)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(5, 3)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(5, 3)).get());

	EXPECT_THROW(tree.findClosestPoint(vector2Df(5, 3)), int) << "Expected error 2 thrown.";

	vector2Df result = tree.findClosestPoint(vector2Df(2, 12))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(5, 3)) <<
			"Expected: " << vector2Df(5, 3) << " Found: " << result;
}

TEST(Tree2DTest, MultiplePointQuery) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(3, 6), vector2Df(17, 15), vector2Df(13, 15), vector2Df(6, 12),
		vector2Df(9, 2), vector2Df(2, 7), vector2Df(10, 19)
	};

	auto testData = testInit(points);
	tree.initializeWithList(testData);
	
	tree.print();

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
		std::vector<const GameObject*> result =
				tree.findKClosestObjects(testPoints[i], expected[i].size());
	
		EXPECT_TRUE(result.size() == expected[i].size()) << "Expected size: " << expected[i].size()
				<< " Found size: " << result.size() << ". Subtest " << i;

		for (int j = 0; j < result.size(); j++) {
			EXPECT_TRUE(result[j]->getPivotPosition() == expected[i][j]) << "Expected: " <<
					expected[i][j] << " Found: " << result[j] << ". Subtest " << i;
		}
	}

	testCleanup(testData);
}

TEST(Tree2DTest, MultiplePoint_DuplicatePoints) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(10, 10), vector2Df(10, 10), vector2Df(10, 10),
		vector2Df(15, 15), vector2Df(15, 15)
	};
	//tree.initializeWithList(points);

	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(15, 15)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(15, 15)).get());
	tree.insert(std::make_unique<MockGameObject>(vector2Df(10, 10)).get());

	tree.print();

	EXPECT_NO_THROW(tree.findKClosestObjects(vector2Df(10, 10), 2));

	EXPECT_THROW(tree.findKClosestObjects(vector2Df(10, 10), 3), int);

	EXPECT_NO_THROW(tree.findKClosestObjects(vector2Df(15, 15), 3));

	EXPECT_THROW(tree.findKClosestObjects(vector2Df(15, 15), 4), int);
}
