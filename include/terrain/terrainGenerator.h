#pragma once

#include <functional>

#include "terrain/terrain.h"

class TerrainGenerator {
public:
	TerrainGenerator(const unsigned int seed, const int generations, const double fillProb);
	TerrainGenerator();

	Terrain generateTerrain(const size_t xSize, const size_t ySize, const size_t shapeSize);
	void setSeed(const unsigned int seed) { this->seed = seed; }
	void setGenerations(const int generations) { this->shapeGenerations = generations; }

private:
	unsigned int seed;

	double shapeFillProb;
	int shapeGenerations;
	size_t blockSize;

	Terrain generateShape(const size_t xSize, const size_t ySize) const;
	unsigned char calculateShape(const size_t x, const size_t y, const Terrain& terrain) const;

	double detailsFillProb;
	std::vector<std::vector<std::array<bool, 4>>> corners;

	/* Finds all corners (empty cell with at least two neighboring filled cells such
	 * that it is in a corner) in the provided Terrain.
	 *
	 * @return	2D vector with a bool array. Use the Corners enum for accessing.
	 */
	std::vector<std::vector<std::array<bool, 4>>> checkCorners(const Terrain& terrain) const;

	Terrain generateDetails(const Terrain& shape);
	/* Generates random terrain for corners in terrain. Requires corners variable to
	 * match the provided terrain.
	 */
	void randomDetails(const size_t x, const size_t y, Terrain& terrain) const;

	int getWallCount(const size_t x, const size_t y, const int range, const Terrain& terrain) const;
	/* Fills every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2
	 * with random values based on the fillProb and current seed.
	 */
	void fillAreaRandom(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
						Terrain& terrain, const double fillProb) const;
	/* Sets every position (x, y) where x1 <= x <= x2 and y1 <= y <= y2 to the output
	 * of the func argument.
	 *
	 * @param func	Callable function returning unsigned char taking in x and y position,
	 *				and a Terrain& to calculate the the value for (x, y).
	 */
	void calculatePortion(
		const size_t x1, const size_t y1, const size_t x2, const size_t y2,
		const Terrain& refTerrain, Terrain& terrain,
		std::function<unsigned char(const size_t, const size_t, const Terrain&)> func) const;

	enum Corners {
		TOP_RIGHT = 0,
		BOT_RIGHT,
		BOT_LEFT,
		TOP_LEFT,
	};
};
