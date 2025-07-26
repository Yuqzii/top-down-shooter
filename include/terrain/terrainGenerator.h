#pragma once

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
	unsigned char calculateShape(const size_t x, const size_t y, Terrain& terrain) const;

	double detailsFillProb;
	std::vector<std::vector<std::array<bool, 4>>> corners;

	// @abstract	Finds all corners (empty cell with at least two neighboring filled cells such
	//				that it is in a corner) in the provided Terrain.
	// @return		2D vector with a bool array. Use the Corners enum for accessing.
	std::vector<std::vector<std::array<bool, 4>>> checkCorners(const Terrain& terrain) const;

	Terrain generateDetails(const Terrain& shape);
	// @abstract	Generates random terrain for corners in terrain. Requires corners variable to
	//				match the provided terrain.
	void randomDetails(const size_t x, const size_t y, Terrain& terrain) const;

	int getWallCount(const size_t x, const size_t y, const int range, const Terrain& terrain) const;
	// @abstract	Fills the terrain from [(x1, y1), (x2, y2)] with random values based on the
	//				current seed.
	void fillAreaRandom(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
						Terrain& terrain, const double fillProb) const;

	enum Corners {
		TOP_RIGHT = 0,
		BOT_RIGHT,
		BOT_LEFT,
		TOP_LEFT,
	};
};
