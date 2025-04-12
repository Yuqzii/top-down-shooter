#include "engine/Tree2D.h"

#include <gtest/gtest.h>

#include "engine/gameObject.h"
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

TEST(Tree2D, General) {
	Tree2D tree;

	std::vector<vector2Df> points = {vector2Df(3, 6),  vector2Df(17, 15), vector2Df(13, 15),
									 vector2Df(6, 12), vector2Df(9, 1),	  vector2Df(2, 7),
									 vector2Df(10, 19)};

	auto testData = testInit(points);
	tree.initializeWithList(testData);

	std::array<vector2Df, 7> testPoints = {vector2Df(10, 10), vector2Df(8, 2),	 vector2Df(4, 7),
										   vector2Df(16, 16), vector2Df(12, 14), vector2Df(0, 5),
										   vector2Df(11, 18)};
	std::array<vector2Df, 7> expected = {vector2Df(6, 12),	vector2Df(9, 1),   vector2Df(3, 6),
										 vector2Df(17, 15), vector2Df(13, 15), vector2Df(2, 7),
										 vector2Df(10, 19)};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestObject(testPoints[i])->getPivotPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testData);
}

TEST(Tree2D, CheckingWithExistingPoints) {
	Tree2D tree;

	std::vector<vector2Df> points = {
		vector2Df(10, 10),
		vector2Df(20, 10),
		vector2Df(10, 5),
		vector2Df(5, 20),
	};

	auto testData = testInit(points);
	tree.initializeWithList(testData);

	std::array<vector2Df, 2> testPoints = {vector2Df(10, 10), vector2Df(10, 5)};

	std::array expected = {vector2Df(10, 5), vector2Df(10, 10)};

	for (int i = 0; i < testPoints.size(); i++) {
		const vector2Df result = tree.findClosestObject(testPoints[i])->getPivotPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testData);
}

TEST(Tree2D, SinglePoint) {
	Tree2D tree;

	auto object = std::make_unique<MockGameObject>(vector2Df(10, 10));
	tree.insert(object.get());

	std::array<vector2Df, 2> testPoints = {vector2Df(10, 10), vector2Df(10, 5)};

	EXPECT_THROW(tree.findClosestObject(testPoints[0]), int) << "Expected error 2 thrown";

	vector2Df expected(10, 10);
	const vector2Df result = tree.findClosestObject(testPoints[1])->getPivotPosition();
	EXPECT_TRUE(result == expected) << "Expected: " << expected << " Found: " << result;
}

TEST(Tree2D, DuplicatePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(4);
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));

	for (auto& obj : objects) {
		tree.insert(obj.get());
	}

	vector2Df result = tree.findClosestObject(vector2Df(5, 2))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(10, 10))
		<< "Expected: " << vector2Df(10, 10) << " Found: " << result;

	result = tree.findClosestObject(vector2Df(1000000000, 1000000000))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(10, 10))
		<< "Expected: " << vector2Df(10, 10) << " Found: " << result;

	result = tree.findClosestObject(vector2Df(10, 10))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(15, 15))
		<< "Expected: " << vector2Df(15, 15) << " Found: " << result;
}

TEST(Tree2D, DuplicateSinglePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(4);
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(5, 3))));

	for (auto& obj : objects) {
		tree.insert(obj.get());
	}

	EXPECT_THROW(tree.findClosestObject(vector2Df(5, 3)), int) << "Expected error 2 thrown.";

	vector2Df result = tree.findClosestObject(vector2Df(2, 12))->getPivotPosition();
	EXPECT_TRUE(result == vector2Df(5, 3))
		<< "Expected: " << vector2Df(5, 3) << " Found: " << result;
}

TEST(Tree2D, MultiplePointQuery) {
	Tree2D tree;

	std::vector<vector2Df> points = {vector2Df(3, 6),  vector2Df(17, 15), vector2Df(13, 15),
									 vector2Df(6, 12), vector2Df(9, 2),	  vector2Df(2, 7),
									 vector2Df(10, 19)};

	auto testData = testInit(points);
	tree.initializeWithList(testData);

	tree.print();

	std::array<vector2Df, 3> testPoints = {vector2Df(2, 5), vector2Df(12, 12), vector2Df(25, 5)};

	std::array<std::vector<vector2Df>, 3> expected = {
		std::vector{vector2Df(3, 6), vector2Df(2, 7), vector2Df(9, 2)},
		std::vector{vector2Df(13, 15), vector2Df(17, 15), vector2Df(6, 12), vector2Df(10, 19)},
		std::vector{vector2Df(17, 15), vector2Df(13, 15), vector2Df(9, 2), vector2Df(6, 12),
					vector2Df(10, 19), vector2Df(3, 6)}};

	for (int i = 0; i < testPoints.size(); i++) {
		const std::vector<GameObject*> result =
			tree.findKClosestObjects(testPoints[i], expected[i].size());

		EXPECT_TRUE(result.size() == expected[i].size())
			<< "Expected size: " << expected[i].size() << " Found size: " << result.size()
			<< ". Subtest " << i;

		for (int j = 0; j < result.size(); j++) {
			EXPECT_TRUE(result[j]->getPivotPosition() == expected[i][j])
				<< "Expected: " << expected[i][j] << " Found: " << result[j] << ". Subtest " << i;
		}
	}

	testCleanup(testData);
}

TEST(Tree2D, MultiplePoint_DuplicatePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(5);
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(vector2Df(10, 10))));

	for (auto& obj : objects) {
		tree.insert(obj.get());
	}

	tree.print();

	EXPECT_NO_THROW(tree.findKClosestObjects(vector2Df(10, 10), 2));

	EXPECT_THROW(tree.findKClosestObjects(vector2Df(10, 10), 3), int);

	EXPECT_NO_THROW(tree.findKClosestObjects(vector2Df(15, 15), 3));

	EXPECT_THROW(tree.findKClosestObjects(vector2Df(15, 15), 4), int);
}

TEST(Tree2D, ObjectsInRange) {
	Tree2D tree;

	std::vector<vector2Df> points = {vector2Df(3, 6),  vector2Df(17, 15), vector2Df(13, 15),
									 vector2Df(6, 12), vector2Df(9, 2),	  vector2Df(2, 7),
									 vector2Df(10, 19)};

	auto testData = testInit(points);
	tree.initializeWithList(testData);

	tree.print();

	std::array<vector2Df, 3> testPoints{vector2Df(12, 12), vector2Df(25, 5), vector2Df(2, 7)};
	std::array<float, testPoints.size()> testRanges{6, 17.5, 10};

	std::array<std::set<vector2Df>, 3> expected{
		std::set{vector2Df(17, 15), vector2Df(13, 15), vector2Df(6, 12)},
		std::set{vector2Df(13, 15), vector2Df(17, 15), vector2Df(9, 2)},
		std::set{vector2Df(3, 6), vector2Df(6, 12), vector2Df(9, 2), vector2Df(2, 7)}};

	for (int i = 0; i < testPoints.size(); i++) {
		const std::vector<GameObject*> result =
			tree.findObjectsInRange(testPoints[i], testRanges[i]);

		EXPECT_TRUE(result.size() == expected[i].size())
			<< "Incorrect result size. Expected: " << expected[i].size()
			<< " Found: " << result.size();

		std::set<vector2Df> resultSet;
		for (auto obj : result) {
			resultSet.insert(obj->getPivotPosition());
		}

		std::cout << "Result: ";
		for (auto a : resultSet) std::cout << a << "  ";
		std::cout << "\nExpected: ";
		for (auto a : expected[i]) std::cout << a << "  ";

		EXPECT_TRUE(resultSet == expected[i]) << "Test " << i;
	}
}

TEST(Tree2D, ObjectsInRange_OtherBranch) {
	Tree2D tree;

	std::vector<vector2Df> points = {vector2Df(9, 15), vector2Df(7, 10), vector2Df(16, 8),
									 vector2Df(9, 9)};

	std::set<vector2Df> expected = {vector2Df(7, 10), vector2Df(9, 9)};

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(points.size());
	for (auto& p : points) {
		objects.push_back(std::make_unique<MockGameObject>(p));
	}

	for (auto& obj : objects) {
		tree.insert(obj.get());
	}

	tree.print();

	const std::vector<GameObject*> result = tree.findObjectsInRange(vector2Df(10, 10), 4);
	std::set<vector2Df> resultPos;
	for (auto& res : result) {
		resultPos.insert(res->getPivotPosition());
	}

	// EXPECT_TRUE(false);
	EXPECT_TRUE(resultPos == expected);
}
