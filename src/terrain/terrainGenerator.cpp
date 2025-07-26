#include "terrain/terrainGenerator.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>

TerrainGenerator::TerrainGenerator(const unsigned int seed, const int shapeGenerations,
								   const double shapeFillProb, const int detailsGenerations,
								   const double cornerFillProb)
	: seed{seed},
	  shapeGenerations{shapeGenerations},
	  shapeFillProb{shapeFillProb},
	  cornerGenerations{detailsGenerations},
	  cornerFillProb{cornerFillProb} {}

TerrainGenerator::TerrainGenerator() : TerrainGenerator{0, 1, 0.5, 1, 0.5} {}

Terrain TerrainGenerator::generateTerrain(const size_t xSize, const size_t ySize,
										  const size_t shapeSize) {
	assert(xSize % shapeSize == 0 && ySize % shapeSize == 0 &&
		   "shapeSize must divide xSize and ySize.");

	srand(seed);

	blockSize = shapeSize;

	Terrain shape = generateShape(xSize / shapeSize, ySize / shapeSize);
	Terrain terrain = generateCorners(shape);
	return terrain;
}

Terrain TerrainGenerator::generateShape(const size_t xSize, const size_t ySize) const {
	Terrain terrain{xSize, ySize};

	// Fill entire terrain randomly
	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			const double result = static_cast<double>(rand()) / RAND_MAX;
			terrain.map[y][x] = result <= shapeFillProb;
		}
	}

	// Calculate the entire terrain per generation
	auto calc = std::bind(&TerrainGenerator::calculateShape, this, std::placeholders::_1,
						  std::placeholders::_2, std::placeholders::_3);
	for (int gen = 0; gen < shapeGenerations; gen++) {
		Terrain curTerrain = terrain;
		calculatePortion(0, 0, xSize - 1, ySize - 1, terrain, curTerrain, calc);
		terrain = curTerrain;
	}

	return terrain;
}

unsigned char TerrainGenerator::calculateShape(const size_t x, const size_t y,
											   const Terrain& terrain) const {
	constexpr int wallsCloseRange = 1, wallsFarRange = 4;
	const int wallsClose = getWallCount(x, y, wallsCloseRange, terrain);
	const int wallsFar = getWallCount(x, y, wallsFarRange, terrain);

	if (wallsClose >= 5 || wallsFar <= 15)
		return 1;
	else
		return 0;
}

Terrain TerrainGenerator::generateCorners(const Terrain& shape) {
	Terrain terrain{shape.getXSize() * blockSize, shape.getYSize() * blockSize};

	corners = checkCorners(shape);
	blockPositions = getBlockPositions(shape);

	for (size_t x = 0; x < shape.getXSize(); x++) {
		for (size_t y = 0; y < shape.getYSize(); y++) {
			if (shape.map[y][x]) {
				fillArea(x * blockSize, y * blockSize,
						 std::min((x + 1) * blockSize, terrain.getXSize() - 1),
						 std::min((y + 1) * blockSize, terrain.getYSize() - 1), terrain, 1);
			}
			randomCorners(x, y, terrain);
		}
	}

	auto calc = std::bind(&TerrainGenerator::calculateCorners, this, std::placeholders::_1,
						  std::placeholders::_2, std::placeholders::_3);
	for (int gen = 0; gen < cornerGenerations; gen++) {
		Terrain curTerrain = terrain;
		for (size_t x = 0; x < shape.getXSize(); x++) {
			for (size_t y = 0; y < shape.getYSize(); y++) {
				if (shape.map[y][x]) continue;

				if (corners[y][x].topRight)
					calculatePortion(blockPositions[y][x].xMid, blockPositions[y][x].top,
									 blockPositions[y][x].right, blockPositions[y][x].yMid, terrain,
									 curTerrain, calc);
				if (corners[y][x].botRight)
					calculatePortion(blockPositions[y][x].xMid, blockPositions[y][x].yMid,
									 blockPositions[y][x].right, blockPositions[y][x].bot, terrain,
									 curTerrain, calc);
				if (corners[y][x].botLeft)
					calculatePortion(blockPositions[y][x].left, blockPositions[y][x].yMid,
									 blockPositions[y][x].xMid, blockPositions[y][x].bot, terrain,
									 curTerrain, calc);
				if (corners[y][x].topLeft)
					calculatePortion(blockPositions[y][x].left, blockPositions[y][x].top,
									 blockPositions[y][x].xMid, blockPositions[y][x].yMid, terrain,
									 curTerrain, calc);
			}
		}
		terrain = curTerrain;
	}

	return terrain;
}

unsigned char TerrainGenerator::calculateCorners(const size_t x, const size_t y,
												 const Terrain& terrain) const {
	constexpr int closeRange = 1;
	constexpr int farRange = 4;
	const int close = getWallCount(x, y, closeRange, terrain);
	const int far = getWallCount(x, y, farRange, terrain);

	if (close >= 3)
		return 1;
	else
		return 0;
}

std::vector<std::vector<TerrainGenerator::Corner>> TerrainGenerator::checkCorners(
	const Terrain& terrain) const {
	std::vector<std::vector<Corner>> result(terrain.getYSize(),
											std::vector<Corner>(terrain.getXSize()));

	for (size_t x = 0; x < terrain.getXSize(); x++) {
		for (size_t y = 0; y < terrain.getYSize(); y++) {
			if (terrain.map[y][x]) continue;

			const bool topRight = x < terrain.getXSize() - 1 && y > 0 && terrain.map[y - 1][x] &&
								  terrain.map[y][x + 1];
			const bool botRight = x < terrain.getXSize() - 1 && y < terrain.getYSize() - 1 &&
								  terrain.map[y + 1][x] && terrain.map[y][x + 1];
			const bool botLeft = x > 0 && y < terrain.getYSize() - 1 && terrain.map[y + 1][x] &&
								 terrain.map[y][x - 1];
			const bool topLeft = x > 0 && y > 0 && terrain.map[y - 1][x] && terrain.map[y][x - 1];

			result[y][x].topRight = topRight;
			result[y][x].botRight = botRight;
			result[y][x].botLeft = botLeft;
			result[y][x].topLeft = topLeft;
		}
	}

	return result;
}

std::vector<std::vector<TerrainGenerator::BlockPosition>> TerrainGenerator::getBlockPositions(
	const Terrain& terrain) const {
	std::vector<std::vector<BlockPosition>> result(terrain.getYSize(),
												   std::vector<BlockPosition>(terrain.getXSize()));

	for (size_t x = 0; x < terrain.getXSize(); x++) {
		for (size_t y = 0; y < terrain.getYSize(); y++) {
			result[y][x].left = x * blockSize;
			result[y][x].xMid = x * blockSize + blockSize / 2;
			result[y][x].right = (x + 1) * blockSize;
			result[y][x].top = y * blockSize;
			result[y][x].yMid = y * blockSize + blockSize / 2;
			result[y][x].bot = (y + 1) * blockSize;
		}
	}

	return result;
}

void TerrainGenerator::randomCorners(const size_t x, const size_t y, Terrain& terrain) const {
	assert(!corners.empty() && x < corners[0].size() && x >= 0 && y < corners.size() && y >= 0 &&
		   "corners must be generated for position x and y.");

	if (corners[y][x].topRight)
		fillAreaRandom(blockPositions[y][x].xMid, blockPositions[y][x].top,
					   blockPositions[y][x].right, blockPositions[y][x].yMid, terrain,
					   cornerFillProb);
	if (corners[y][x].botRight)
		fillAreaRandom(blockPositions[y][x].xMid, blockPositions[y][x].yMid,
					   blockPositions[y][x].right, blockPositions[y][x].bot, terrain,
					   cornerFillProb);
	if (corners[y][x].botLeft)
		fillAreaRandom(blockPositions[y][x].left, blockPositions[y][x].yMid,
					   blockPositions[y][x].xMid, blockPositions[y][x].bot, terrain,
					   cornerFillProb);
	if (corners[y][x].topLeft)
		fillAreaRandom(blockPositions[y][x].left, blockPositions[y][x].top,
					   blockPositions[y][x].xMid, blockPositions[y][x].yMid, terrain,
					   cornerFillProb);
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

void TerrainGenerator::fillArea(const size_t x1, const size_t y1, const size_t x2, const size_t y2,
								Terrain& terrain, const unsigned char value) const {
	assert(x1 >= 0 && x1 < terrain.getXSize() && y1 >= 0 && y2 < terrain.getYSize() &&
		   "Coordinates must be within the terrain size.");

	for (size_t y = y1; y <= y2; y++) {
		std::fill(terrain.map[y].begin() + x1, terrain.map[y].begin() + x2, value);
	}
}

void TerrainGenerator::calculatePortion(
	const size_t x1, const size_t y1, const size_t x2, const size_t y2, const Terrain& refTerrain,
	Terrain& terrain,
	std::function<unsigned char(const size_t, const size_t, const Terrain&)> func) const {
	assert(refTerrain.getXSize() == terrain.getXSize() &&
		   refTerrain.getYSize() == terrain.getYSize() &&
		   "refTerrain and terrain must be the same size.");
	assert(x1 >= 0 && x1 < terrain.getXSize() && y1 >= 0 && y1 < terrain.getYSize() &&
		   "Coordinates must be inside the terrain size.");

	for (size_t x = x1; x <= x2; x++) {
		for (size_t y = y1; y <= y2; y++) {
			terrain.map[y][x] = func(x, y, refTerrain);
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
