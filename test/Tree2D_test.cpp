#include "engine/Tree2D.h"

#include <gtest/gtest.h>

#include <ranges>

#include "engine/gameObject.h"
#include "mockGameObject.h"

std::vector<GameObject*> makePtrVec(const std::vector<Vec2>& points) {
	std::vector<MockGameObject*> objects;
	objects.reserve(points.size());
	for (const Vec2& point : points) {
		objects.push_back(new MockGameObject{point});
	}

	return std::vector<GameObject*>(objects.begin(), objects.end());
}

std::vector<std::reference_wrapper<GameObject>> makeReference(
    const std::vector<GameObject*>& ptrs) {
	auto range =
	    ptrs | std::views::transform([](GameObject* ptr) -> std::reference_wrapper<GameObject> { return std::ref(*ptr); });
	return std::vector<std::reference_wrapper<GameObject>>{range.begin(), range.end()};
}

void testCleanup(std::vector<GameObject*>& objects) {
	for (GameObject* obj : objects) {
		delete obj;
	}
}

TEST(Tree2D, General) {
	Tree2D tree;

	std::vector<Vec2> points = {Vec2(3, 6), Vec2(17, 15), Vec2(13, 15), Vec2(6, 12),
	                            Vec2(9, 1), Vec2(2, 7),   Vec2(10, 19)};

	auto testDataPtr = makePtrVec(points);
	auto testData = makeReference(testDataPtr);
	tree.initializeWithList(testData);

	std::array<Vec2, 7> testPoints = {Vec2(10, 10), Vec2(8, 2), Vec2(4, 7),  Vec2(16, 16),
	                                  Vec2(12, 14), Vec2(0, 5), Vec2(11, 18)};
	std::array<Vec2, 7> expected = {Vec2(6, 12),  Vec2(9, 1), Vec2(3, 6),  Vec2(17, 15),
	                                Vec2(13, 15), Vec2(2, 7), Vec2(10, 19)};

	for (int i = 0; i < testPoints.size(); i++) {
		const Vec2 result = tree.findClosestObject(testPoints[i])->getPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testDataPtr);
}

TEST(Tree2D, CheckingWithExistingPoints) {
	Tree2D tree;

	std::vector<Vec2> points = {
	    Vec2(10, 10),
	    Vec2(20, 10),
	    Vec2(10, 5),
	    Vec2(5, 20),
	};

	auto testDataPtr = makePtrVec(points);
	auto testData = makeReference(testDataPtr);
	tree.initializeWithList(testData);

	std::array<Vec2, 2> testPoints = {Vec2(10, 10), Vec2(10, 5)};

	std::array expected = {Vec2(10, 5), Vec2(10, 10)};

	for (int i = 0; i < testPoints.size(); i++) {
		const Vec2 result = tree.findClosestObject(testPoints[i])->getPosition();
		EXPECT_TRUE(result == expected[i]) << "Expected: " << expected[i] << " Found: " << result;
	}

	testCleanup(testDataPtr);
}

TEST(Tree2D, SinglePoint) {
	Tree2D tree;

	auto object = std::make_unique<MockGameObject>(Vec2(10, 10));
	tree.insert(*object);

	std::array<Vec2, 2> testPoints = {Vec2(10, 10), Vec2(10, 5)};

	EXPECT_THROW(tree.findClosestObject(testPoints[0]), int) << "Expected error 2 thrown";

	Vec2 expected(10, 10);
	const Vec2 result = tree.findClosestObject(testPoints[1])->getPosition();
	EXPECT_TRUE(result == expected) << "Expected: " << expected << " Found: " << result;
}

TEST(Tree2D, DuplicatePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(4);
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));

	for (auto& obj : objects) {
		tree.insert(*obj);
	}

	Vec2 result = tree.findClosestObject(Vec2(5, 2))->getPosition();
	EXPECT_TRUE(result == Vec2(10, 10)) << "Expected: " << Vec2(10, 10) << " Found: " << result;

	result = tree.findClosestObject(Vec2(1000000000, 1000000000))->getPosition();
	EXPECT_TRUE(result == Vec2(10, 10)) << "Expected: " << Vec2(10, 10) << " Found: " << result;

	result = tree.findClosestObject(Vec2(10, 10))->getPosition();
	EXPECT_TRUE(result == Vec2(15, 15)) << "Expected: " << Vec2(15, 15) << " Found: " << result;
}

TEST(Tree2D, DuplicateSinglePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(4);
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(5, 3))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(5, 3))));

	for (auto& obj : objects) {
		tree.insert(*obj);
	}

	EXPECT_THROW(tree.findClosestObject(Vec2(5, 3)), int) << "Expected error 2 thrown.";

	Vec2 result = tree.findClosestObject(Vec2(2, 12))->getPosition();
	EXPECT_TRUE(result == Vec2(5, 3)) << "Expected: " << Vec2(5, 3) << " Found: " << result;
}

TEST(Tree2D, MultiplePointQuery) {
	Tree2D tree;

	std::vector<Vec2> points = {Vec2(3, 6), Vec2(17, 15), Vec2(13, 15), Vec2(6, 12),
	                            Vec2(9, 2), Vec2(2, 7),   Vec2(10, 19)};

	auto testDataPtr = makePtrVec(points);
	auto testData = makeReference(testDataPtr);
	tree.initializeWithList(testData);

	std::array<Vec2, 3> testPoints = {Vec2(2, 5), Vec2(12, 12), Vec2(25, 5)};

	std::array<std::vector<Vec2>, 3> expected = {
	    std::vector{Vec2(3, 6), Vec2(2, 7), Vec2(9, 2)},
	    std::vector{Vec2(13, 15), Vec2(17, 15), Vec2(6, 12), Vec2(10, 19)},
	    std::vector{Vec2(17, 15), Vec2(13, 15), Vec2(9, 2), Vec2(6, 12), Vec2(10, 19), Vec2(3, 6)}};

	for (int i = 0; i < testPoints.size(); i++) {
		const std::vector<std::reference_wrapper<GameObject>> result =
		    tree.findKClosestObjects(testPoints[i], expected[i].size());

		EXPECT_TRUE(result.size() == expected[i].size())
		    << "Expected size: " << expected[i].size() << " Found size: " << result.size()
		    << ". Subtest " << i;

		for (int j = 0; j < result.size(); j++) {
			EXPECT_TRUE(result[j].get().getPosition() == expected[i][j])
			    << "Expected: " << expected[i][j] << " Found: " << result[j].get().getPosition()
			    << ". Subtest " << i;
		}
	}

	testCleanup(testDataPtr);
}

TEST(Tree2D, MultiplePoint_DuplicatePoints) {
	Tree2D tree;

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(5);
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(15, 15))));
	objects.push_back(std::move(std::make_unique<MockGameObject>(Vec2(10, 10))));

	for (auto& obj : objects) {
		tree.insert(*obj);
	}

	EXPECT_NO_THROW(tree.findKClosestObjects(Vec2(10, 10), 2));

	EXPECT_THROW(tree.findKClosestObjects(Vec2(10, 10), 3), int);

	EXPECT_NO_THROW(tree.findKClosestObjects(Vec2(15, 15), 3));

	EXPECT_THROW(tree.findKClosestObjects(Vec2(15, 15), 4), int);
}

TEST(Tree2D, ObjectsInRange) {
	Tree2D tree;

	std::vector<Vec2> points = {Vec2(3, 6), Vec2(17, 15), Vec2(13, 15), Vec2(6, 12),
	                            Vec2(9, 2), Vec2(2, 7),   Vec2(10, 19)};

	auto testDataPtr = makePtrVec(points);
	auto testData = makeReference(testDataPtr);
	tree.initializeWithList(testData);

	std::array<Vec2, 3> testPoints{Vec2(12, 12), Vec2(25, 5), Vec2(2, 7)};
	std::array<float, testPoints.size()> testRanges{6, 17.5, 10};

	std::array<std::set<Vec2>, 3> expected{
	    std::set{Vec2(17, 15), Vec2(13, 15), Vec2(6, 12)},
	    std::set{Vec2(13, 15), Vec2(17, 15), Vec2(9, 2)},
	    std::set{Vec2(3, 6), Vec2(6, 12), Vec2(9, 2), Vec2(2, 7)}};

	for (int i = 0; i < testPoints.size(); i++) {
		const std::vector<std::reference_wrapper<GameObject>> result =
		    tree.findObjectsInRange(testPoints[i], testRanges[i]);

		EXPECT_TRUE(result.size() == expected[i].size())
		    << "Incorrect result size. Expected: " << expected[i].size()
		    << " Found: " << result.size();

		std::set<Vec2> resultSet;
		for (GameObject& obj : result) {
			resultSet.insert(obj.getPosition());
		}

		EXPECT_TRUE(resultSet == expected[i]) << "Test " << i;
	}
}

TEST(Tree2D, ObjectsInRange_OtherBranch) {
	Tree2D tree;

	std::vector<Vec2> points = {Vec2(9, 15), Vec2(7, 10), Vec2(16, 8), Vec2(9, 9)};

	std::set<Vec2> expected = {Vec2(7, 10), Vec2(9, 9)};

	std::vector<std::unique_ptr<MockGameObject>> objects;
	objects.reserve(points.size());
	for (auto& p : points) {
		objects.push_back(std::make_unique<MockGameObject>(p));
	}

	for (auto& obj : objects) {
		tree.insert(*obj);
	}

	const std::vector<std::reference_wrapper<GameObject>> result =
	    tree.findObjectsInRange(Vec2(10, 10), 4);
	std::set<Vec2> resultPos;
	for (GameObject& res : result) {
		resultPos.insert(res.getPosition());
	}

	// EXPECT_TRUE(false);
	EXPECT_TRUE(resultPos == expected);
}
