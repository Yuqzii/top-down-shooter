#pragma once

#include "terrain/terrain.h"

class TerrainGenerator {
public:
	TerrainGenerator(const unsigned int seed, const int generations, const double fillProb);
	TerrainGenerator();

	Terrain generateTerrain(const size_t xSize, const size_t ySize, const size_t shapeSize) const;
	void setSeed(const unsigned int seed) { this->seed = seed; }
	void setGenerations(const int generations) { this->shapeGenerations = generations; }

private:
	unsigned int seed;

	double shapeFillProb;
	int shapeGenerations;
	Terrain generateShape(const size_t xSize, const size_t ySize) const;
	unsigned char calculatePortion(const size_t x, const size_t y, Terrain& terrain) const;

	double detailsFillProb;
	Terrain generateDetails(const Terrain& shape, const size_t shapeSize) const;
	// Generates random terrain if it is in a corner,
	// for example both above and to the right are filled.
	void randomDetails(const size_t x, const size_t y, const size_t blockSize, const Terrain& shape,
					   Terrain& terrain) const;

	int getWallCount(const size_t x, const size_t y, const int range, const Terrain& terrain) const;
	// Fills the terrain from [(x1, y1), (x2, y2)] with random values based on the current seed.
	void fillAreaRandom(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
						Terrain& terrain, const double fillProb) const;
};
