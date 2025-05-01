#include "engine/collision.h"

#include <gtest/gtest.h>

using namespace Collision;

TEST(Collision, Line_Circle) {
	const Circle c(Vec2(10, 10), 5);

	const std::array<Line, 6> testLines = {
		Line(Vec2(5, 5), Vec2(10, 20)),	 Line(Vec2(10, 20), Vec2(5, 5)),
		Line(Vec2(4, 10), Vec2(10, 19)), Line(Vec2(10, 1), Vec2(2, 4)),
		Line(Vec2(8, 4), Vec2(9, 10)),	 Line(Vec2(8, 8), Vec2(10, 13))};
	const std::array<bool, testLines.size()> expected = {true, true, true, false, true, true};

	for (int i = 0; i < testLines.size(); i++) {
		EXPECT_TRUE(checkCollision(c, testLines[i]).collided == expected[i])
			<< "Line: " << testLines[i].start << " to " << testLines[i].end;
	}
};
