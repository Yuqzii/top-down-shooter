#include "terrain/terrainGenerator.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <random>

TerrainGenerator::TerrainGenerator(std::mt19937& randGen)
	: randGen{randGen},
	  shapeFillProb{0.5},
	  shapeGenerations{1},
	  shapeConsecutiveWallRange{1},
	  shapeMinConsecutiveWall{1},
	  shapeWallRandomness{0.5},
	  shapeCalcCloseRange{1},
	  shapeCalcFarRange{3},
	  shapeCalcMinCloseFill{4},
	  shapeCalcMaxFarFill{8},
	  cornerFillProb{0.5},
	  cornerGenerations{1},
	  cornerCalcRange{1},
	  cornerCalcMinFill{4},
	  detailsGenerations{1},
	  detailsCalcRange{1},
	  detailsCalcMinFill{4},
	  edgeThickness{50} {}

Terrain TerrainGenerator::generateTerrain(const std::size_t xSize, const std::size_t ySize,
										  const std::size_t shapeSize) {
	assert(xSize % shapeSize == 0 && ySize % shapeSize == 0 &&
		   "shapeSize must divide xSize and ySize.");

	blockSize = shapeSize;

	Terrain shape = generateShape(xSize / shapeSize, ySize / shapeSize);
	Terrain corners = generateCorners(shape);
	Terrain edges = addEdges(corners);
	Terrain details = generateDetails(edges);
	return details;
}

Terrain TerrainGenerator::generateShape(const std::size_t xSize, const std::size_t ySize) {
	Terrain terrain{xSize, ySize};

	// Fill entire terrain randomly
	std::uniform_real_distribution<double> dist{0, 1};
	for (std::size_t x = 0; x < xSize; x++) {
		for (std::size_t y = 0; y < ySize; y++) {
			const double result = dist(randGen);
			terrain.map[y][x] = result <= shapeFillProb;
		}
	}

	// Calculate the entire terrain per generation
	auto calc = std::bind_front(&TerrainGenerator::calculateShape, this);
	for (int gen = 0; gen < shapeGenerations; gen++) {
		Terrain curTerrain = terrain;
		calculateArea(0, 0, xSize - 1, ySize - 1, terrain, curTerrain, calc);
		terrain = curTerrain;
	}

	for (std::size_t x = 1; x < xSize - 1; x++) {
		for (std::size_t y = 1; y < ySize - 1; y++) {
			terrain.map[y][x] =
				randomizeConsecutiveWall(x, y, shapeConsecutiveWallRange, shapeMinConsecutiveWall,
										 shapeWallRandomness, terrain);
		}
	}

	return terrain;
}

unsigned char TerrainGenerator::calculateShape(const std::size_t x, const std::size_t y,
											   const Terrain& terrain) const {
	const int wallsClose = getWallCount(x, y, shapeCalcCloseRange, terrain);
	const int wallsFar = getWallCount(x, y, shapeCalcFarRange, terrain);

	return wallsClose >= shapeCalcMinCloseFill || wallsFar <= shapeCalcMaxFarFill;
}

Terrain TerrainGenerator::generateCorners(const Terrain& shape) {
	Terrain terrain{shape.getXSize() * blockSize, shape.getYSize() * blockSize};

	corners = checkCorners(shape);
	blockPositions = getBlockPositions(shape);

	for (std::size_t x = 0; x < shape.getXSize(); x++) {
		for (std::size_t y = 0; y < shape.getYSize(); y++) {
			if (shape.map[y][x]) {
				fillArea(x * blockSize, y * blockSize,
						 std::min((x + 1) * blockSize, terrain.getXSize() - 1),
						 std::min((y + 1) * blockSize, terrain.getYSize() - 1), terrain, 1);
			}
			randomCorners(x, y, terrain);
		}
	}

	auto calc = std::bind_front(&TerrainGenerator::calculateCorners, this);
	for (int gen = 0; gen < cornerGenerations; gen++) {
		Terrain curTerrain = terrain;
		for (std::size_t x = 0; x < shape.getXSize(); x++) {
			for (std::size_t y = 0; y < shape.getYSize(); y++) {
				if (corners[y][x].topRight)
					calculateArea(blockPositions[y][x].xMid, blockPositions[y][x].top,
								  blockPositions[y][x].right, blockPositions[y][x].yMid, terrain,
								  curTerrain, calc);
				if (corners[y][x].botRight)
					calculateArea(blockPositions[y][x].xMid, blockPositions[y][x].yMid,
								  blockPositions[y][x].right, blockPositions[y][x].bot, terrain,
								  curTerrain, calc);
				if (corners[y][x].botLeft)
					calculateArea(blockPositions[y][x].left, blockPositions[y][x].yMid,
								  blockPositions[y][x].xMid, blockPositions[y][x].bot, terrain,
								  curTerrain, calc);
				if (corners[y][x].topLeft)
					calculateArea(blockPositions[y][x].left, blockPositions[y][x].top,
								  blockPositions[y][x].xMid, blockPositions[y][x].yMid, terrain,
								  curTerrain, calc);
			}
		}
		terrain = curTerrain;
	}

	return terrain;
}

unsigned char TerrainGenerator::calculateCorners(const std::size_t x, const std::size_t y,
												 const Terrain& terrain) const {
	const int count = getWallCount(x, y, cornerCalcRange, terrain);

	return count >= cornerCalcMinFill;
}

std::vector<std::vector<TerrainGenerator::Corner>> TerrainGenerator::checkCorners(
	const Terrain& terrain) const {
	std::vector<std::vector<Corner>> result(terrain.getYSize(),
											std::vector<Corner>(terrain.getXSize()));

	for (std::size_t x = 0; x < terrain.getXSize(); x++) {
		for (std::size_t y = 0; y < terrain.getYSize(); y++) {
			const bool above = y > 0 && terrain.map[y - 1][x];
			const bool below = y < terrain.getYSize() - 1 && terrain.map[y + 1][x];
			const bool right = x < terrain.getXSize() - 1 && terrain.map[y][x + 1];
			const bool left = x > 0 && terrain.map[y][x - 1];
			if (terrain.map[y][x]) {
				result[y][x].topRight = !above && !right;
				result[y][x].botRight = !below && !right;
				result[y][x].botLeft = !below && !left;
				result[y][x].topLeft = !above && !left;
			} else {
				result[y][x].topRight = above && right;
				result[y][x].botRight = below && right;
				result[y][x].botLeft = below && left;
				result[y][x].topLeft = above && left;
			}
		}
	}

	return result;
}

std::vector<std::vector<TerrainGenerator::BlockPosition>> TerrainGenerator::getBlockPositions(
	const Terrain& terrain) const {
	std::vector<std::vector<BlockPosition>> result(terrain.getYSize(),
												   std::vector<BlockPosition>(terrain.getXSize()));

	for (std::size_t x = 0; x < terrain.getXSize(); x++) {
		for (std::size_t y = 0; y < terrain.getYSize(); y++) {
			result[y][x].left = x * blockSize;
			result[y][x].xMid = x * blockSize + blockSize / 2;
			result[y][x].right = (x + 1) * blockSize - 1;
			result[y][x].top = y * blockSize;
			result[y][x].yMid = y * blockSize + blockSize / 2;
			result[y][x].bot = (y + 1) * blockSize - 1;
		}
	}

	return result;
}

void TerrainGenerator::randomCorners(const std::size_t x, const std::size_t y, Terrain& terrain) {
	assert(!corners.empty() && x < corners[0].size() && x >= 0 && y < corners.size() && y >= 0 &&
		   "Corners must be generated for position X and Y.");

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

Terrain TerrainGenerator::generateDetails(const Terrain& reference) const {
	Terrain terrain = reference;

	auto calc = std::bind_front(&TerrainGenerator::calculateDetails, this);
	for (int gen = 0; gen < detailsGenerations; gen++) {
		Terrain curTerrain = terrain;
		calculateArea(0, 0, terrain.getXSize() - 1, terrain.getYSize() - 1, terrain, curTerrain,
					  calc);
		terrain = curTerrain;
	}

	return terrain;
}

unsigned char TerrainGenerator::calculateDetails(const std::size_t x, const std::size_t y,
												 const Terrain& terrain) const {
	constexpr int closeRange = 2;
	const int count = getWallCount(x, y, detailsCalcRange, terrain);

	return count >= detailsCalcMinFill;
}

Terrain TerrainGenerator::addEdges(const Terrain& reference) const {
	Terrain terrain{reference.getXSize() + edgeThickness * 2,
					reference.getYSize() + edgeThickness * 2};

	for (std::size_t x = 0; x < terrain.getXSize(); x++) {
		for (std::size_t y = 0; y < terrain.getYSize(); y++) {
			if (x < edgeThickness || y < edgeThickness ||
				x >= terrain.getXSize() - edgeThickness - 1 ||
				y >= terrain.getYSize() - edgeThickness)
				terrain.map[y][x] = 1;
			else
				terrain.map[y][x] = reference.map[y - edgeThickness][x - edgeThickness];
		}
	}

	return terrain;
}

void TerrainGenerator::fillAreaRandom(const std::size_t x1, const std::size_t y1,
									  const std::size_t x2, const std::size_t y2, Terrain& terrain,
									  const double fillProb) {
	assert(x1 >= 0 && x2 < terrain.getXSize() &&
		   "X coordinates must be in the interval [0, xSize].");
	assert(y1 >= 0 && y2 < terrain.getYSize() &&
		   "Y coordinates must be in the interval [0, ySize].");

	std::uniform_real_distribution<double> dist{0, 1};
	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			const double result = dist(randGen);
			terrain.map[y][x] = result <= fillProb;
		}
	}
}

void TerrainGenerator::fillArea(const std::size_t x1, const std::size_t y1, const std::size_t x2,
								const std::size_t y2, Terrain& terrain,
								const unsigned char value) const {
	assert(x1 >= 0 && x1 < terrain.getXSize() && y1 >= 0 && y2 < terrain.getYSize() &&
		   "Coordinates must be within the terrain size.");

	for (std::size_t y = y1; y <= y2; y++) {
		std::fill(terrain.map[y].begin() + x1, terrain.map[y].begin() + x2, value);
	}
}

void TerrainGenerator::calculateArea(
	const std::size_t x1, const std::size_t y1, const std::size_t x2, const std::size_t y2,
	const Terrain& refTerrain, Terrain& terrain,
	std::function<unsigned char(const std::size_t, const std::size_t, const Terrain&)> func) const {
	assert(refTerrain.getXSize() == terrain.getXSize() &&
		   refTerrain.getYSize() == terrain.getYSize() &&
		   "refTerrain and terrain must be the same size.");
	assert(x1 >= 0 && x1 < terrain.getXSize() && y1 >= 0 && y1 < terrain.getYSize() &&
		   "Coordinates must be inside the terrain size.");

	for (std::size_t x = x1; x <= x2; x++) {
		for (std::size_t y = y1; y <= y2; y++) {
			terrain.map[y][x] = func(x, y, refTerrain);
		}
	}
}

unsigned char TerrainGenerator::randomizeConsecutiveWall(const std::size_t x, const std::size_t y,
														 const int range, const int wallLength,
														 const double prob,
														 const Terrain& terrain) {
	assert(y < terrain.getYSize() - 1 && y > 0 &&
		   "There must be at least one cell above and below.");
	assert(x < terrain.getXSize() - 1 && x > 0 &&
		   "There must be at least one cell to the left and right.");

	auto checkVertical = [&terrain, x](const std::size_t y) -> bool {
		const bool above = terrain.map[y - 1][x];
		const bool right = terrain.map[y][x + 1];
		const bool below = terrain.map[y + 1][x];
		const bool left = terrain.map[y][x - 1];

		return ((above && below) && (left != right));
	};
	int vertical = 0;
	for (std::size_t v = y; v >= std::max(y - range, static_cast<std::size_t>(1)); v--) {
		if (checkVertical(v))
			++vertical;
		else
			break;
	}
	for (std::size_t v = y + 1; v <= std::min(y + range, terrain.getYSize() - 2); v++) {
		if (checkVertical(v))
			++vertical;
		else
			break;
	}

	if (vertical >= wallLength) {
		std::uniform_real_distribution<double> dist{0, 1};
		const double result = dist(randGen);
		return result <= prob;
	}

	auto checkHorizontal = [&terrain, y](const std::size_t x) -> bool {
		const bool above = terrain.map[y - 1][x];
		const bool right = terrain.map[y][x + 1];
		const bool below = terrain.map[y + 1][x];
		const bool left = terrain.map[y][x - 1];

		return ((left && right) && (above != below));
	};
	int horizontal = 0;
	for (std::size_t h = x; h >= std::max(x - range, static_cast<std::size_t>(1)); h--) {
		if (checkHorizontal(h))
			++horizontal;
		else
			break;
	}
	for (std::size_t h = x + 1; h <= std::min(x + range, terrain.getXSize() - 2); h++) {
		if (checkHorizontal(h))
			++horizontal;
		else
			break;
	}

	if (horizontal >= wallLength) {
		std::uniform_real_distribution<double> dist{0, 1};
		const double result = dist(randGen);
		return result <= prob;
	}

	return terrain.map[y][x];
}

int TerrainGenerator::getWallCount(const std::size_t midX, const std::size_t midY, const int range,
								   const Terrain& terrain) const {
	int result = 0;
	const std::size_t startX = std::max(static_cast<std::size_t>(0), midX - range);
	const std::size_t endX = std::min(terrain.map[0].size() - 1, midX + range);
	const std::size_t startY = std::max(static_cast<std::size_t>(0), midY - range);
	const std::size_t endY = std::min(terrain.map.size() - 1, midY + range);

	for (std::size_t x = startX; x <= endX; x++) {
		for (std::size_t y = startY; y <= endY; y++) {
			if (x == midX && y == midY) continue;
			result += terrain.map[y][x] == 1;
		}
	}

	return result;
}
