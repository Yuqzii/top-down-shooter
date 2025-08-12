#include "terrain/chunkManager.h"

#include <cassert>
#include <cmath>
#include <cstddef>

#include "SDL2/SDL_render.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/chunk.h"
#include "terrain/terrainCollider.h"

ChunkManager::ChunkManager(const Terrain& terrain, const std::size_t chunkSize,
                           const int pixelSizeMultiplier, const SDL_Color& color_, Scene& scene_)
    : chunkSize{chunkSize},
      pixelSize{Game::pixelSize * pixelSizeMultiplier},
      color{color_},
      scene{scene_},
      chunks{splitToChunks(terrain, chunkSize)},
      terrainXSize{terrain.getXSize()},
      terrainYSize{terrain.getYSize()} {}

ChunkManager::~ChunkManager() = default;

void ChunkManager::update(const Vec2& playerPos) {
	if (!pendingTerrainChanges.empty()) executeTerrainChanges();

	updateActiveChunks(playerPos, chunkRange);
	for (Chunk& chunk : activeChunks) chunk.update(scene);
}

void ChunkManager::collisionUpdate() {
	for (Chunk& chunk : activeChunks) chunk.collisionUpdate(scene);
}

void ChunkManager::updateRender() {
	for (auto& vec : chunks)
		for (Chunk& chunk : vec) chunk.updateRender(pixelSize);
}

void ChunkManager::updateColliders() {
	for (auto& vec : chunks)
		for (Chunk& chunk : vec) chunk.updateColliders();
}

void ChunkManager::updateActiveChunks(const Vec2& pos, const int range) {
	const auto [x, y] = posToChunk(posToTerrainCoord(pos));
	activeChunks = getChunksInRange(x, y, range);
}

void ChunkManager::render(SDL_Renderer* renderer, const Camera& cam) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (const Chunk& chunk : activeChunks) chunk.render(renderer, cam);
}

void ChunkManager::changeTerrain(const Vec2& position, const unsigned char value) {
	const auto pixelPos = posToTerrainCoord(position);
	changeTerrain(pixelPos, value);
}

void ChunkManager::changeTerrain(const std::pair<std::size_t, std::size_t>& position,
                                 const unsigned char value) {
	changeTerrain(position.first, position.second, value);
}

void ChunkManager::changeTerrain(const std::size_t x, const std::size_t y,
                                 const unsigned char value) {
	if (x >= terrainXSize || y >= terrainYSize) return;
	pendingTerrainChanges.emplace(x, y, value);
}

void ChunkManager::changeTerrainInRange(const Vec2& center, int range, const unsigned char value) {
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
			changeTerrain(cX + x, cY + y, value);
		}
	}
}

void ChunkManager::executeTerrainChanges() {
	std::map<std::pair<std::size_t, std::size_t>, std::vector<TerrainChange>> chunkMap;

	while (!pendingTerrainChanges.empty()) {
		TerrainChange change = std::move(pendingTerrainChanges.front());
		pendingTerrainChanges.pop();
		auto chunkPos = posToChunk(std::make_pair(change.x, change.y));
		auto localX = change.x % chunkSize;
		auto localY = change.y % chunkSize;
		chunkMap[chunkPos].emplace_back(localX, localY, change.value);
	}

	for (auto& [chunk, changes] : chunkMap) {
		auto [x, y] = chunk;
		chunks[y][x].changeTerrainMultiple(changes);
	}
}

std::pair<std::size_t, std::size_t> ChunkManager::posToTerrainCoord(const Vec2& position) const {
	const std::size_t x = position.x / pixelSize;
	const std::size_t y = position.y / pixelSize;
	return std::make_pair(x, y);
}

std::pair<std::size_t, std::size_t> ChunkManager::posToChunk(
    const std::pair<std::size_t, std::size_t>& pos) const {
	assert(chunkSize != 0);

	const std::size_t x = pos.first / chunkSize;
	const std::size_t y = pos.second / chunkSize;
	return std::move(std::make_pair(x, y));
}

std::vector<std::vector<Chunk>> ChunkManager::splitToChunks(const Terrain& terrain,
                                                            const std::size_t chunkSize) {
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

			result[y].emplace_back(std::move(chunkMap), x * chunkSize * pixelSize,
			                       y * chunkSize * pixelSize, *this);
		}
	}
	return result;
}

std::vector<std::reference_wrapper<Chunk>> ChunkManager::getChunksInRange(const std::size_t midX,
                                                                          const std::size_t midY,
                                                                          const int range) {
	std::vector<std::reference_wrapper<Chunk>> result;
	const std::size_t minX = (midX > range) ? midX - range : 0;
	const std::size_t maxX = std::min(midX + range, getChunksX() - 1);
	const std::size_t minY = (midY > range) ? midY - range : 0;
	const std::size_t maxY = std::min(midY + range, getChunksY() - 1);

	for (std::size_t x = minX; x <= maxX; x++)
		for (std::size_t y = minY; y <= maxY; y++) result.push_back(chunks[y][x]);

	return result;
}

std::vector<std::reference_wrapper<const Chunk>> ChunkManager::getConstChunksInRange(
    const std::size_t midX, const std::size_t midY, const int range) const {
	std::vector<std::reference_wrapper<const Chunk>> result;
	const std::size_t minX = std::max(midX - range, static_cast<std::size_t>(0));
	const std::size_t maxX = std::min(midX + range, getChunksX() - 1);
	const std::size_t minY = std::max(midY - range, static_cast<std::size_t>(0));
	const std::size_t maxY = std::min(midY + range, getChunksY() - 1);

	for (std::size_t x = minX; x <= maxX; x++)
		for (std::size_t y = minY; y <= maxY; y++) result.push_back(chunks[y][x]);

	return result;
}

std::vector<Vec2> ChunkManager::getAllSpawns() const {
	std::vector<Vec2> spawns;
	for (const auto& chunkList : chunks) {
		for (const Chunk& chunk : chunkList) {
			const auto chunkSpawns = chunk.getSpawnPositions();
			spawns.insert(spawns.end(), chunkSpawns.begin(), chunkSpawns.end());
		}
	}
	return spawns;
}
