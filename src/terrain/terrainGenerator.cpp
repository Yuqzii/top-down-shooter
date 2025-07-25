#include "terrain/terrainGenerator.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

TerrainGenerator::TerrainGenerator(const unsigned int seed, const int generations,
								   const double fillProb)
	: seed{seed}, shapeGenerations{generations}, shapeFillProb{fillProb} {}

TerrainGenerator::TerrainGenerator() : TerrainGenerator{0, 1, 0.5} {}

Terrain TerrainGenerator::generateTerrain(const size_t xSize, const size_t ySize,
										  const size_t shapeSize) const {
	assert(xSize % shapeSize == 0 && ySize % shapeSize == 0 &&
		   "shapeSize must divide xSize and ySize.");

	Terrain shape = generateShape(xSize / shapeSize, ySize / shapeSize);
	Terrain terrain = generateDetails(shape, shapeSize);
	return generateShape(xSize, ySize);
}

Terrain TerrainGenerator::generateShape(const size_t xSize, const size_t ySize) const {
	Terrain terrain{xSize, ySize};
	srand(seed);

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			const double result = static_cast<double>(rand()) / RAND_MAX;
			terrain.map[y][x] = result <= shapeFillProb;
		}
	}

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			std::cout << (terrain.map[y][x] == 1 ? "#" : ".");
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	for (int gen = 0; gen < shapeGenerations; gen++) {
		auto curMap = terrain.map;
		for (size_t x = 0; x < xSize; x++) {
			for (size_t y = 0; y < ySize; y++) {
				curMap[y][x] = calculatePortion(x, y, terrain);
				std::cout << (curMap[y][x] == 1 ? "#" : ".");
			}
			std::cout << "\n";
		}
		terrain.map = curMap;
		std::cout << "\n";
	}

	return terrain;
}

unsigned char TerrainGenerator::calculatePortion(const size_t x, const size_t y,
												 Terrain& terrain) const {
	constexpr int wallsCloseRange = 1, wallsFarRange = 4;
	const int wallsClose = getWallCount(x, y, wallsCloseRange, terrain);
	const int wallsFar = getWallCount(x, y, wallsFarRange, terrain);

	if (wallsClose >= 5 || wallsFar <= 15)
		return 1;
	else
		return 0;
}

Terrain TerrainGenerator::generateDetails(const Terrain& shape, const size_t blockSize) const {
	const size_t xSize = shape.getXSize();
	const size_t ySize = shape.getYSize();
	Terrain terrain{xSize * blockSize, ySize * blockSize};

	for (size_t x = 0; x < xSize; x++) {
		for (size_t y = 0; y < ySize; y++) {
			if (shape.map[y][x] == 1) continue;
			randomDetails(x, y, blockSize, shape, terrain);
		}
	}

	return terrain;
}

void TerrainGenerator::randomDetails(const size_t x, const size_t y, const size_t blockSize,
									 const Terrain& shape, Terrain& terrain) const {
	const size_t xSize = shape.getXSize();
	const size_t ySize = shape.getYSize();

	const bool topRight = x < xSize - 1 && y > 0 && shape.map[y - 1][x] && shape.map[y][x + 1];
	const bool botRight =
		x < xSize - 1 && y < ySize - 1 && shape.map[y + 1][x] && shape.map[y][x + 1];
	const bool botLeft = x > 0 && y < ySize - 1 && shape.map[y + 1][x] && shape.map[y][x - 1];
	const bool topLeft = x > 0 && y > 0 && shape.map[y - 1][x] && shape.map[y][x - 1];

	const size_t left = x * blockSize;
	const size_t xMid = x * blockSize + blockSize / 2;
	const size_t right = (x + 1) * blockSize;
	const size_t top = y * blockSize;
	const size_t yMid = y * blockSize + blockSize / 2;
	const size_t bot = (y + 1) * blockSize;

	if (topRight) fillAreaRandom(xMid, top, right, yMid, terrain, detailsFillProb);
	if (botRight) fillAreaRandom(xMid, yMid, right, bot, terrain, detailsFillProb);
	if (botLeft) fillAreaRandom(left, yMid, xMid, bot, terrain, detailsFillProb);
	if (topLeft) fillAreaRandom(left, top, xMid, yMid, terrain, detailsFillProb);
}

void TerrainGenerator::fillAreaRandom(const size_t x1, const size_t y1, const size_t x2,
									  const size_t y2, Terrain& terrain,
									  const double fillProb) const {
	assert(x1 >= 0 && x2 < terrain.getXSize() &&
		   "x coordinates must be in the interval [0, xSize].");
	assert(y1 >= 0 && y2 < terrain.getYSize() &&
		   "y coordinates must be in the interval [0, ySize].");

	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			const double result = static_cast<double>(rand()) / RAND_MAX;
			terrain.map[y][x] = result <= fillProb;
		}
	}
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
