#include <gtest/gtest.h>
#include "engine/collision.h"

using namespace Collision;

TEST(Collision, Line_Circle) {
	const Circle c(vector2D(10, 10), 5);
	
	const std::array<Line, 6> testLines = {
		Line(vector2D(5, 5), vector2D(10, 20)), Line(vector2D(10, 20), vector2D(5, 5)),
		Line(vector2D(4, 10), vector2D(10, 19)), Line(vector2D(10, 1), vector2D(2, 4)),
		Line(vector2D(8, 4), vector2D(9, 10)), Line(vector2D(8, 8), vector2D(10, 13))
	};
	const std::array<bool, testLines.size()> expected = { true, true, true, false, true, true };

	for (int i = 0; i < testLines.size(); i++) {
		EXPECT_TRUE(checkCollision(c, testLines[i]) == expected[i])
				<< "Line: " << testLines[i].start << " to " << testLines[i].end;
	}
};
