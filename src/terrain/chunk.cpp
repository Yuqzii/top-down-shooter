#include "terrain/chunk.h"

#include <cassert>

#include "terrain/terrainManager.h"

Chunk::Chunk(const std::vector<std::vector<unsigned char>>& map, TerrainManager& manager)
	: terrain{map}, manager{manager} {}

void Chunk::setCell(const std::size_t x, const std::size_t y, const unsigned char value) {
	assert(x > 0 && x < terrain.getXSize() && y > 0 && y < terrain.getYSize() &&
		   "Position (x, y) must be within the terrain size.");

	terrain.map[y][x] = value;
}

void Chunk::updateRender(const int pixelSize) {
	for (std::size_t x = 0; x < terrain.getXSize(); x++) {
		for (std::size_t y = 0; y < terrain.getYSize(); y++) {
			if (terrain.map[y][x]) {
				if (renderRects[y][x].w == pixelSize) continue;	 // Already calculated

				renderRects[y][x].x = x * pixelSize;
				renderRects[y][x].y = y * pixelSize;
				renderRects[y][x].w = renderRects[y][x].h = pixelSize;
			} else {
				// Set width and height to 0 so that it is not rendered
				renderRects[y][x].w = renderRects[y][x].h = 0;
			}
		}
	}
}

void Chunk::updateColliders() {
	std::map<std::pair<int, int>, std::pair<int, int>> currentColliders;  // Key: end, Value: start

	for (std::size_t x = 0; x < terrain.getXSize(); ++x) {
		const float xPos = x * manager.getPixelSize();
		for (std::size_t y = 0; y < terrain.getYSize(); ++y) {
			if (!terrain.map[x][y]) continue;

			const std::size_t yPos = y * manager.getPixelSize();
			const std::pair<int, int> topLeft{xPos, yPos};
			const std::pair<int, int> topRight{xPos + manager.getPixelSize(), yPos};
			const std::pair<int, int> botLeft{xPos, yPos + manager.getPixelSize()};
			const std::pair<int, int> botRight{xPos + manager.getPixelSize(),
											   yPos + manager.getPixelSize()};

			const bool left = x > 0 && terrain.map[x - 1][y];
			const bool right = x < terrain.getXSize() - 1 && terrain.map[x + 1][y];
			const bool above = y > 0 && terrain.map[x][y - 1];
			const bool below = y < terrain.getYSize() - 1 && terrain.map[x][y + 1];

			if (left && right && above && below)
				continue;  // Terrain in all directions
			else if (!left && right && above && below) {
				// Only empty to the left
				tryExtendCollider(topLeft, botLeft, currentColliders);
			} else if (left && !right && above && below) {
				// Only empty to the right
				tryExtendCollider(topRight, botRight, currentColliders);
			} else if (left && right && !above && below) {
				// Only empty above
				tryExtendCollider(topLeft, topRight, currentColliders);
			} else if (left && right && above && !below) {
				// Only empty below
				tryExtendCollider(botLeft, botRight, currentColliders);
			} else if (!left && !right && above && below) {
				// Straight vertical line
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
			} else if ((!left && right && !above && below) || (left && !right && above && !below)) {
				// Diagonal line from bottom left to top right
				tryExtendCollider(botLeft, topRight, currentColliders);
			} else if ((!left && right && above && !below) || (left && !right && !above && below)) {
				// Diagonal from top left to bottom right
				tryExtendCollider(topLeft, botRight, currentColliders);
			} else if (left && right && !above && !below) {
				// Straight horizontal line
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			} else if (!left && !right && !above && below) {
				// Three lines, vertical left, horizontal above, and vertical right
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
			} else if (!left && !right && above && !below) {
				// Three line, vertical left, vertical right, and horizontal below
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			} else if (!left && right && !above && !below) {
				// Three lines, vertical left, horizontal above, and horizontal below
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			} else if (left && !right && !above && !below) {
				// Three lines, vertical right, horizontal above, and horizontal below
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			} else if (!left && !right && !above && !below) {
				// Collidersr on every side
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
		}
	}

	// Construct colliders
	for (const auto& [end, start] : currentColliders)
		manager.createCollider(Vec2{start.first, start.second}, Vec2{end.first, end.second});
}

void Chunk::tryExtendCollider(
	const std::pair<int, int>& start, const std::pair<int, int>& end,
	std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders) {
	const Vec2 startVec{start.first, start.second};
	const Vec2 endVec{end.first, end.second};

	// Create collider if there already is one ending at end
	if (currentColliders.count(end)) {
		manager.createCollider(Vec2{currentColliders[end].first, currentColliders[end].second},
							   endVec);
		currentColliders.erase(end);
	}

	// Is there a collider ending at start?
	if (currentColliders.count(start)) {
		const Vec2 curStartVec{currentColliders[start].first, currentColliders[start].second};

		// Change end point if line is going the same direction
		if ((startVec - curStartVec).normalized() == (endVec - startVec).normalized()) {
			auto node = currentColliders.extract(start);
			node.key() = end;
			currentColliders.insert(std::move(node));
			return;
		}
	}
	currentColliders[end] = start;
}
