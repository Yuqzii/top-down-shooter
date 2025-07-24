#include "terrain/terrainGenerator.h"

#include <cstdlib>
#include <iostream>

TerrainGenerator::TerrainGenerator(const unsigned int seed, const int generations,
								   const double fillProb)
	: seed{seed}, generations{generations}, fillProb{fillProb} {}

TerrainGenerator::TerrainGenerator() : TerrainGenerator{0, 1, 0.5} {}

Terrain TerrainGenerator::generateTerrain(const size_t xSize, const size_t ySize) const {
	Terrain terrain{xSize, ySize};
	srand(seed);

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			const double result = static_cast<double>(rand()) / RAND_MAX;
			terrain.map[y][x] = result <= fillProb;
		}
	}

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			std::cout << (terrain.map[y][x] == 1 ? "# " : ". ");
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	for (int gen = 0; gen < generations; gen++) {
		auto curMap = terrain.map;
		for (size_t x = 0; x < xSize; x++) {
			for (size_t y = 0; y < ySize; y++) {
				curMap[y][x] = calculatePixel(x, y, terrain);
				std::cout << (curMap[y][x] == 1 ? "# " : ". ");
			}
			std::cout << "\n";
		}
		terrain.map = curMap;
		std::cout << "\n";
	}

	return terrain;
}

unsigned char TerrainGenerator::calculatePixel(const size_t x, const size_t y,
											   Terrain& terrain) const {
	const int wallsClose = getWallCount(x, y, 1, terrain);
	const int wallsFar = getWallCount(x, y, 4, terrain);

	if (wallsClose >= 5 || wallsFar <= 15)
		return 1;
	else
		return 0;
}

int TerrainGenerator::getWallCount(const size_t midX, const size_t midY, const int range,
								   const Terrain& terrain) const {
	int result = 0;
	const size_t startX = std::max(static_cast<size_t>(0), midX - range);
	const size_t endX = std::min(terrain.map[0].size() - 1, midX + range);
	const size_t startY = std::max(static_cast<size_t>(0), midY - range);
	const size_t endY = std::min(terrain.map.size() - 1, midY + range);

	for (size_t x = startX; x <= endX; x++) {
		for (size_t y = startY; y <= endY; y++) {
			if (x == midX && y == midY) continue;
			result += terrain.map[y][x] == 1;
		}
	}

	return result;
}
