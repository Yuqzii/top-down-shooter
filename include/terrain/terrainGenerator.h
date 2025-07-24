#pragma once

#include "terrain/terrain.h"

class TerrainGenerator {
public:
	TerrainGenerator(const unsigned int seed, const int generations, const double fillProb);
	TerrainGenerator();

	Terrain generateTerrain(const size_t xSize, const size_t ySize) const;
	void setSeed(const unsigned int seed) { this->seed = seed; }
	void setGenerations(const int generations) { this->generations = generations; }

private:
	unsigned int seed;
	double fillProb;
	int generations;

	unsigned char calculatePixel(const size_t x, const size_t y, Terrain& terrain) const;
	int getWallCount(const size_t x, const size_t y, const int range, const Terrain& terrain) const;
};
