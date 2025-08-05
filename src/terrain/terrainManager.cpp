#include "terrain/terrainManager.h"

#include <cassert>
#include <cmath>
#include <cstddef>

#include "SDL2/SDL_render.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/chunk.h"
#include "terrain/terrainCollider.h"

TerrainManager::TerrainManager(const Terrain& terrain, const std::size_t chunkSize,
							   const int pixelSizeMultiplier, const SDL_Color& color_,
							   Scene& scene_)
	: chunkSize{chunkSize},
	  pixelSize{Game::pixelSize * pixelSizeMultiplier},
	  color{color_},
	  scene{scene_},
	  chunks{std::move(splitToChunks(terrain, chunkSize))},
	  terrainXSize{terrain.getXSize()},
	  terrainYSize{terrain.getYSize()} {
	updateRender();
	updateColliders();
}

void TerrainManager::updateRender() {
	for (auto& vec : chunks)
		for (auto& chunk : vec) chunk.updateRender(pixelSize);
}

void TerrainManager::updateColliders() {
	// Remove previous colliders
	for (auto collider : terrainColliders) collider->deleteObject = true;
	terrainColliders.clear();

	for (auto& vec : chunks)
		for (auto& chunk : vec) chunk.updateColliders();

	updateTree();
}

void TerrainManager::updateTree() {
	terrainTree = Tree2D{};
	terrainTree.initializeWithList(terrainColliders);
}

void TerrainManager::render(SDL_Renderer* renderer, const Camera& cam) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (auto& chunkList : chunks) {
		for (auto& chunk : chunkList) chunk.render(renderer, cam);
	}
}

void TerrainManager::setCell(const Vec2& position, const unsigned char value) {
	const auto pixelPos = posToTerrainCoord(position);
	setCell(pixelPos, value);
}

void TerrainManager::setCell(const std::pair<std::size_t, std::size_t>& position,
							 const unsigned char value) {
	auto [x, y] = position;
	if (x >= terrainXSize || y >= terrainYSize) return;

	auto [chunkX, chunkY] = posToChunk(position);
	assert(chunkX >= 0 && chunkX < getChunksX() && chunkY >= 0 && chunkY < getChunksY());
	chunks[chunkY][chunkX].setCell(x % chunkSize, y % chunkSize, value);
}

void TerrainManager::setCell(const std::size_t x, const std::size_t y, const unsigned char value) {
	setCell(std::make_pair(x, y), value);
}

void TerrainManager::setCellsInRange(const Vec2& center, int range, const unsigned char value) {
	++range;  // Increment range to make calculations work correctly

	// Find the height (sine) of every x position based on the range (radius)
	std::vector<int> maxY(range);
	for (int x = 0; x < range; ++x) {
		maxY[x] = std::round(std::sin(std::acos((float)x / range)) * range);
	}

	const auto [cX, cY] = posToTerrainCoord(center);
	std::map<std::pair<std::size_t, std::size_t>, std::vector<std::pair<std::size_t, std::size_t>>>
		chunkMap;
	// Loop through and deactivate pixels
	for (int x = -range + 1; x < range; ++x) {
		if (cX + x < 0 || cX + x >= terrainXSize) continue;
		for (int y = -maxY[std::abs(x)] + 1; y < maxY[std::abs(x)]; ++y) {
			if (cY + y < 0 || cY + y >= terrainYSize) continue;

			auto pos = posToChunk(std::make_pair(cX + x, cY + y));
			auto chunkPos = std::make_pair((cX + x) % chunkSize, (cY + y) % chunkSize);
			chunkMap[pos].push_back(chunkPos);
		}
	}

	for (auto [chunk, posList] : chunkMap) {
		auto [x, y] = chunk;
		chunks[y][x].setCellMultiple(posList, value);
	}
}

std::pair<std::size_t, std::size_t> TerrainManager::posToTerrainCoord(const Vec2& position) const {
	const std::size_t x = position.x / pixelSize;
	const std::size_t y = position.y / pixelSize;
	return std::make_pair(x, y);
}

std::pair<std::size_t, std::size_t> TerrainManager::posToChunk(
	const std::pair<std::size_t, std::size_t>& pos) const {
	assert(chunkSize != 0);

	const std::size_t x = pos.first / chunkSize;
	const std::size_t y = pos.second / chunkSize;
	return std::move(std::make_pair(x, y));
}

std::vector<std::vector<Chunk>> TerrainManager::splitToChunks(const Terrain& terrain,
															  const int chunkSize) {
	assert(terrain.getXSize() % chunkSize == 0 && terrain.getYSize() % chunkSize == 0 &&
		   "chunkSize must divide terrain x- and y-size.");

	const std::size_t chunksX = terrain.getXSize() / chunkSize;
	const std::size_t chunksY = terrain.getYSize() / chunkSize;
	std::vector<std::vector<Chunk>> result(chunksY);

	for (std::size_t y = 0; y < chunksY; y++) {
		result[y].reserve(chunksX);
		for (std::size_t x = 0; x < chunksX; x++) {
			// Copy this chunk's part of the terrain map and initialize the chunk with it
			std::vector<std::vector<unsigned char>> chunkMap(chunkSize,
															 std::vector<unsigned char>(chunkSize));
			for (int chunkY = y * chunkSize, localY = 0; chunkY < (y + 1) * chunkSize;
				 chunkY++, localY++) {
				std::copy(terrain.map[chunkY].begin() + x * chunkSize,
						  terrain.map[chunkY].begin() + (x + 1) * chunkSize,
						  chunkMap[localY].begin());
			}

			result[y].emplace_back(chunkMap, x * chunkSize * pixelSize, y * chunkSize * pixelSize, *this);
		}
	}
	return result;
}

std::vector<Vec2> TerrainManager::getAllSpawns() const {
	std::vector<Vec2> spawns;
	for (const auto& chunkList : chunks) {
		for (const Chunk& chunk : chunkList) {
			const auto chunkSpawns = chunk.getSpawnPositions();
			spawns.insert(spawns.end(), chunkSpawns.begin(), chunkSpawns.end());
		}
	}
	return spawns;
}
