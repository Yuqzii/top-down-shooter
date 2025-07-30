#include "terrain/terrainManager.h"

#include <cassert>

#include "SDL2/SDL_render.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/chunk.h"
#include "terrain/terrainCollider.h"

TerrainManager::TerrainManager(const Terrain& terrain, const int chunkSize,
							   const int pixelSizeMultiplier, const SDL_Color& color_,
							   Scene& scene_)
	: pixelSize{Game::pixelSize * pixelSizeMultiplier}, color{color_}, scene{scene_} {
	renderRects.resize(terrain.getYSize());
	for (auto& rectList : renderRects) rectList.resize(terrain.getXSize());
}

void TerrainManager::updateRender() {
	for (auto& vec : chunks)
		for (auto& chunk : vec) chunk->updateRender(pixelSize);
}

void TerrainManager::updateColliders() {
	// Remove previous colliders
	for (auto collider : terrainColliders) collider->deleteObject = true;
	terrainColliders.clear();

	for (auto& vec : chunks)
		for (auto& chunk : vec) chunk->updateColliders();

	updateTree();
}

void TerrainManager::createCollider(const Vec2& start, const Vec2& end) {
	const Vec2 position{start + (end - start) * 0.5f};
	TerrainCollider& collider = scene.instantiate<TerrainCollider>(position, start, end, this);
	terrainColliders.push_back(&collider);
}

void TerrainManager::updateTree() {
	terrainTree = Tree2D{};
	terrainTree.initializeWithList(terrainColliders);
}

void TerrainManager::render(SDL_Renderer* renderer, const Camera& cam) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (auto& rectList : renderRects) {
		std::vector<SDL_Rect> rects{rectList};
		for (auto& rect : rects) {
			const Vec2 camPos = cam.getPos();
			rect.x -= camPos.x;
			rect.y -= camPos.y;
		}
		SDL_RenderFillRects(renderer, &rects[0], rects.size());
	}
}

void TerrainManager::removePixel(const Vec2& position) {
	const auto pixelPos = posToTerrainCoord(position);
	removePixel(pixelPos);
}

void TerrainManager::removePixel(const std::pair<int, int>& position) {
	const auto [x, y] = position;
	if (x >= xSize || y >= ySize || !terrain.map[x][y]) return;

	terrain.map[x][y] = false;
	updateRender();
	updateColliders();
}

void TerrainManager::removeInRange(const Vec2& center, int range) {
	++range;  // Increment range to make calculations work correctly

	// Find the height (sine) of every x position based on the range (radius)
	std::vector<int> maxY(range);
	for (int x = 0; x < range; ++x) {
		maxY[x] = std::round(std::sin(std::acos((float)x / range)) * range);
	}

	const auto [cX, cY] = posToTerrainCoord(center);  // Get center to remove from
	// Loop through and deactivate pixels
	for (int x = -range + 1; x < range; ++x) {
		if (cX + x < 0 || cX + x >= xSize) continue;
		for (int y = -maxY[std::abs(x)] + 1; y < maxY[std::abs(x)]; ++y) {
			if (cY + y < 0 || cY + y >= ySize) continue;
			terrain.map[cX + x][cY + y] = false;
		}
	}

	updateRender();
	updateColliders();
}

std::pair<int, int> TerrainManager::posToTerrainCoord(const Vec2& position) const {
	const int x = position.x / pixelSize;
	const int y = position.y / pixelSize;
	return std::move(std::pair<int, int>{x, y});
}

std::vector<std::vector<std::unique_ptr<Chunk>>> splitToChunks(const Terrain& terrain,
															   const int chunkSize) {
	assert(terrain.getXSize() % chunkSize == 0 && terrain.getYSize() % chunkSize == 0 &&
		   "chunkSize must divide terrain x- and y-size.");

	std::vector<std::vector<std::unique_ptr<Chunk>>> result(terrain.getYSize() / chunkSize);
}
