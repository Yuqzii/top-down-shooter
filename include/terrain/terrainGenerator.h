#pragma once

#include <functional>
#include <random>

#include "terrain/terrain.h"

class TerrainManager;

class TerrainGenerator {
public:
	TerrainGenerator(std::mt19937& randGen);

	Terrain generateTerrain(const std::size_t xSize, const std::size_t ySize,
	                        const std::size_t shapeSize);

	double shapeFillProb;
	int shapeGenerations;
	int shapeConsecutiveWallRange;
	int shapeMinConsecutiveWall;
	double shapeWallRandomness;
	int shapeCalcCloseRange;
	int shapeCalcFarRange;
	// Minimum amount of filled cells within close range to fill the current cell.
	int shapeCalcMinCloseFill;
	// Maximum amount of filled cells within far range to fill the current cell.
	int shapeCalcMaxFarFill;

	double cornerFillProb;
	int cornerGenerations;
	int cornerCalcRange;
	// Minimum amount of filled cells within range to fill the current cell.
	int cornerCalcMinFill;

	int detailsGenerations;
	int detailsCalcRange;
	// Minimum amount of filled cells within range to fill the current cell.
	int detailsCalcMinFill;

	// Added in addition to the size specified when calling generateTerrain.
	int edgeThickness;

private:
	struct Corner {
		bool topRight;
		bool botRight;
		bool botLeft;
		bool topLeft;
	};

	struct Neighbors {
		bool above;
		bool right;
		bool below;
		bool left;
	};

	struct BlockPosition {
		std::size_t left, xMid, right;
		std::size_t top, yMid, bot;
	};

	std::size_t blockSize;

	std::mt19937& randGen;

	Terrain generateShape(const std::size_t xSize, const std::size_t ySize);
	unsigned char calculateShape(const std::size_t x, const std::size_t y,
	                             const Terrain& terrain) const;

	std::vector<std::vector<BlockPosition>> blockPositions;
	std::vector<std::vector<Corner>> corners;
	/* Finds all corners in the provided Terrain. Corners are empty cell with at least two
	 * neighboring filled cells such that it is an inner corner, or a filled cell that have at least
	 * two neighboring unfilled cells such that it is an outer corner.
	 *
	 * @return Two dimensional vector of Corner struct.
	 */
	std::vector<std::vector<Corner>> checkCorners(const Terrain& terrain) const;
	std::vector<std::vector<BlockPosition>> getBlockPositions(const Terrain& terrain) const;

	Terrain generateCorners(const Terrain& shape);
	/* Generates random terrain for corners in terrain. Requires corners variable to
	 * match the provided terrain.
	 */
	void randomCorners(const std::size_t x, const std::size_t y, Terrain& terrain);
	unsigned char calculateCorners(const std::size_t x, const std::size_t y,
	                               const Terrain& terrain) const;

	Terrain generateDetails(Terrain terrain) const;
	unsigned char calculateDetails(const std::size_t x, const std::size_t y,
	                               const Terrain& terrain) const;

	Terrain addEdges(const Terrain& reference) const;

	int getWallCount(const std::size_t x, const std::size_t y, const int range,
	                 const Terrain& terrain) const;
	/* Fills every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2
	 * with random values based on the fillProb and current seed.
	 */
	void fillAreaRandom(const std::size_t x1, const std::size_t y1, const std::size_t x2,
	                    const std::size_t y2, Terrain& terrain, const double fillProb);
	void fillArea(const std::size_t x1, const std::size_t y1, const std::size_t x2,
	              const std::size_t y2, Terrain& terrain, const unsigned char value) const;
	/* Sets every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2 to the output
	 * of the func parameter.
	 *
	 * @param func	Callable function returning unsigned char taking in x and y position,
	 *				and a Terrain& to calculate the the value for (x, y).
	 */
	void calculateArea(
	    const std::size_t x1, const std::size_t y1, const std::size_t x2, const std::size_t y2,
	    const Terrain& refTerrain, Terrain& terrain,
	    std::function<unsigned char(const std::size_t, const std::size_t, const Terrain&)> func)
	    const;

	/* Checks if the location (x, y) is part of a straight horizontal or vertical wall with minimum
	 * length wallLength.
	 *
	 * @param range			How far in each direction to check for walls.
	 * @param wallLength	Minimum length the wall must have for the randomization to occur.
	 * @param prob			The probability of the wall remaining a wall if it matches the previous
	 *						requirements.
	 */
	unsigned char randomizeConsecutiveWall(const std::size_t x, const std::size_t y,
	                                       const int range, const int wallLength, const double prob,
	                                       const Terrain& terrain);

	inline Neighbors getNeighbors(const std::size_t x, const std::size_t y,
	                              const Terrain& terrain) const;
};
