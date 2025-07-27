#pragma once

#include <functional>

#include "terrain/terrain.h"

class TerrainGenerator {
public:
	TerrainGenerator(const unsigned int seed, const int shapeGenerations,
					 const double shapeFillProb, const int cornerGenerations,
					 const double cornerFillProb);
	TerrainGenerator();

	Terrain generateTerrain(const size_t xSize, const size_t ySize, const size_t shapeSize);

	void setSeed(const unsigned int seed) { this->seed = seed; }
	void setShapeFillProb(const double prob) { this->shapeFillProb = prob; }
	void setShapeGenerations(const int generations) { this->shapeGenerations = generations; }
	void setShapeConsecutiveWallRange(const int range) { this->shapeConsecutiveWallRange = range; }
	void setShapeMinConsecutiveWall(const int value) { this->shapeMinConsecutiveWall = value; }
	void setShapeWallRandomness(const double value) { this->shapeWallRandomness = value; }

	void setCornerFillProb(const double prob) { this->cornerFillProb = prob; }
	void setCornerGenerations(const int generations) { this->cornerGenerations = generations; }

	void setDetailsGenerations(const int generations) { this->detailsGenerations = generations; }

private:
	struct Corner {
		bool topRight;
		bool botRight;
		bool botLeft;
		bool topLeft;
	};

	struct BlockPosition {
		size_t left, xMid, right;
		size_t top, yMid, bot;
	};

	unsigned int seed;

	double shapeFillProb;
	int shapeGenerations;
	int shapeConsecutiveWallRange;
	int shapeMinConsecutiveWall;
	double shapeWallRandomness;
	size_t blockSize;
	std::vector<std::vector<BlockPosition>> blockPositions;

	Terrain generateShape(const size_t xSize, const size_t ySize) const;
	unsigned char calculateShape(const size_t x, const size_t y, const Terrain& terrain) const;

	double cornerFillProb;
	int cornerGenerations;
	std::vector<std::vector<Corner>> corners;

	int detailsGenerations;

	/* Finds all corners (empty cell with at least two neighboring filled cells such
	 * that it is in a corner) in the provided Terrain.
	 *
	 * @return	2D vector with a bool array. Use the Corners enum for accessing.
	 */
	std::vector<std::vector<Corner>> checkCorners(const Terrain& terrain) const;
	std::vector<std::vector<BlockPosition>> getBlockPositions(const Terrain& terrain) const;

	Terrain generateCorners(const Terrain& shape);
	/* Generates random terrain for corners in terrain. Requires corners variable to
	 * match the provided terrain.
	 */
	void randomCorners(const size_t x, const size_t y, Terrain& terrain) const;
	unsigned char calculateCorners(const size_t x, const size_t y, const Terrain& terrain) const;

	Terrain generateDetails(const Terrain& reference) const;
	unsigned char calculateDetails(const size_t x, const size_t y, const Terrain& terrain) const;

	int getWallCount(const size_t x, const size_t y, const int range, const Terrain& terrain) const;
	/* Fills every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2
	 * with random values based on the fillProb and current seed.
	 */
	void fillAreaRandom(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
						Terrain& terrain, const double fillProb) const;
	void fillArea(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
				  Terrain& terrain, const unsigned char value) const;
	/* Sets every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2 to the output
	 * of the func parameter.
	 *
	 * @param func	Callable function returning unsigned char taking in x and y position,
	 *				and a Terrain& to calculate the the value for (x, y).
	 */
	void calculatePortion(
		const size_t x1, const size_t y1, const size_t x2, const size_t y2,
		const Terrain& refTerrain, Terrain& terrain,
		std::function<unsigned char(const size_t, const size_t, const Terrain&)> func) const;

	/* Checks if the location (x, y) is part of a straight horizontal or vertical wall with minimum
	 * length wallLength.
	 *
	 * @param range			How far in each direction to check for walls.
	 * @param wallLength	Minimum length the wall must have for the randomization to occur.
	 * @param prob			The probability of the wall remaining a wall if it matches the previous
	 *						requirements.
	 */
	unsigned char randomizeConsecutiveWall(const size_t x, const size_t y, const int range,
										   const int wallLength, const double prob,
										   const Terrain& terrain) const;
};
