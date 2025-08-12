#include "terrain/chunk.h"

#include <cassert>

#include "engine/camera.h"
#include "terrain/terrainCollider.h"
#include "terrain/chunkManager.h"

std::array<int, Chunk::minSpawnSpace> Chunk::spawnCircleY = [] {
	std::array<int, minSpawnSpace> result;
	for (int x = 0; x < minSpawnSpace; x++) {
		result[x] =
		    std::round(std::sin(std::acos(static_cast<double>(x) / minSpawnSpace)) * minSpawnSpace);
	}
	return result;
}();

Chunk::Chunk(std::vector<std::vector<unsigned char>>&& map, const std::size_t originX,
             const std::size_t originY, ChunkManager& manager)
    : manager{manager},
      terrain{std::move(map)},
      originX{originX},
      originY{originY},
      renderRects{},
      colliders{} {
	renderRects.resize(terrain.getYSize(), std::vector<SDL_Rect>(terrain.getXSize()));
	updateColliders();
	updateSpawnPositions();
	updateRender(manager.getPixelSize());
}

void Chunk::update(Scene& scene) {
	for (TerrainCollider& collider : colliders) collider.update(scene);
}

void Chunk::collisionUpdate(Scene& scene) {
	for (TerrainCollider& collider : colliders) collider.collisionUpdate(scene);
}

void Chunk::changeTerrain(const TerrainChange& change) {
	assert(change.x >= 0 && change.x < terrain.getXSize() && change.y >= 0 &&
	       change.y < terrain.getYSize() && "Position (x, y) must be within the terrain size.");

	terrain.map[change.y][change.x] = change.value;

	updateColliders();
	updateRender(manager.getPixelSize());
}

void Chunk::changeTerrainMultiple(const std::vector<TerrainChange>& changes) {
	for (auto [x, y, value] : changes) {
		assert(x >= 0 && x < terrain.getXSize() && y >= 0 && y < terrain.getYSize() &&
		       "Position (x, y) must be within the terrain size.");
		terrain.map[y][x] = value;
	}

	updateColliders();
	updateRender(manager.getPixelSize());
}

void Chunk::render(SDL_Renderer* renderer, const Camera& cam) const {
	std::vector<SDL_Rect> rects;
	rects.reserve(terrain.getXSize() * terrain.getYSize());
	for (auto curRects : renderRects) {
		for (auto& rect : curRects) {
			const Vec2& camPos = cam.getPos();
			rect.x -= camPos.x;
			rect.y -= camPos.y;
		}
		rects.insert(rects.end(), curRects.begin(), curRects.end());
	}
	SDL_RenderFillRects(renderer, &rects[0], rects.size());
}

void Chunk::updateRender(const int pixelSize) {
	for (std::size_t x = 0; x < terrain.getXSize(); x++) {
		for (std::size_t y = 0; y < terrain.getYSize(); y++) {
			if (terrain.map[y][x]) {
				if (renderRects[y][x].w == pixelSize) continue;  // Already calculated

				renderRects[y][x].x = x * pixelSize + originX;
				renderRects[y][x].y = y * pixelSize + originY;
				renderRects[y][x].w = renderRects[y][x].h = pixelSize;
			} else {
				// Set width and height to 0 so that it is not rendered
				renderRects[y][x].w = renderRects[y][x].h = 0;
			}
		}
	}
}

void Chunk::updateColliders() {
	colliders.clear();

	std::map<std::pair<int, int>, std::pair<int, int>> currentColliders;  // Key: end, Value: start

	for (std::size_t x = 0; x < terrain.getXSize(); ++x) {
		const float xPos = x * manager.getPixelSize() + originX;
		for (std::size_t y = 0; y < terrain.getYSize(); ++y) {
			if (!terrain.map[y][x]) continue;

			const std::size_t yPos = y * manager.getPixelSize() + originY;
			const std::pair<int, int> topLeft{xPos, yPos};
			const std::pair<int, int> topRight{xPos + manager.getPixelSize(), yPos};
			const std::pair<int, int> botLeft{xPos, yPos + manager.getPixelSize()};
			const std::pair<int, int> botRight{xPos + manager.getPixelSize(),
			                                   yPos + manager.getPixelSize()};

			const bool left = x > 0 && terrain.map[y][x - 1];
			const bool right = x < terrain.getXSize() - 1 && terrain.map[y][x + 1];
			const bool above = y > 0 && terrain.map[y - 1][x];
			const bool below = y < terrain.getYSize() - 1 && terrain.map[y + 1][x];

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
				// Colliders on every side
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
		}
	}

	// Construct colliders
	colliders.reserve(colliders.size() + currentColliders.size());
	for (const auto& [end, start] : currentColliders)
		createCollider(Vec2{start.first, start.second}, Vec2{end.first, end.second});
}

void Chunk::tryExtendCollider(
    const std::pair<int, int>& start, const std::pair<int, int>& end,
    std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders) {
	const Vec2 startVec{start.first, start.second};
	Vec2 endVec{end.first, end.second};

	// Create collider if there already is one ending at end
	auto endIt = currentColliders.find(end);
	if (endIt != currentColliders.end()) {
		createCollider(Vec2{endIt->second.first, endIt->second.second}, std::move(endVec));
		currentColliders.erase(endIt);
	}

	// Is there a collider ending at start?
	auto startIt = currentColliders.find(start);
	if (startIt != currentColliders.end()) {
		const Vec2 curStartVec{startIt->second.first, startIt->second.second};

		// Change end point if line is going the same direction
		if ((startVec - curStartVec).normalized() == (endVec - startVec).normalized()) {
			auto node = currentColliders.extract(startIt);
			node.key() = end;
			currentColliders.insert(std::move(node));
			return;
		}
	}
	currentColliders[end] = start;
}

void Chunk::createCollider(Vec2&& start, Vec2&& end) {
	Vec2 position{start + (end - start) * 0.5f};
	colliders.emplace_back(std::move(position), std::move(start), std::move(end), *this);
}

void Chunk::updateSpawnPositions() {
	spawnPositions.clear();

	Terrain used{terrain.map};
	for (std::size_t y = minSpawnSpace; y < terrain.getYSize() - minSpawnSpace; y++) {
		for (std::size_t x = minSpawnSpace; x < terrain.getXSize() - minSpawnSpace; x++) {
			auto result = findObstruction(x, y, used);
			if (result.has_value()) {
				// Move just enough to the right to avoid whatever we encountered.
				auto [hitX, hitY] = result.value();
				// hitX + minSpawnSpace moves so that the entire area is outside,
				// the y stuff moves back proportional to how high up we hit.
				x = hitX + minSpawnSpace - (std::max(hitY, y) - std::min(hitY, y)) +
				    spawnCircleY.back() + 1;
			} else {
				spawnPositions.push_back(Vec2{x * manager.getPixelSize() + originX,
				                              y * manager.getPixelSize() + originY});

				// Set all positions inside the spawn area as used.
				const int cornerDist = spawnCircleY.back() - 1;
				for (int xAdd = -minSpawnSpace + 1; xAdd < minSpawnSpace; xAdd++) {
					for (int yAdd = -spawnCircleY[std::abs(xAdd)];
					     yAdd < spawnCircleY[std::abs(xAdd)]; yAdd++) {
						used.map[y + yAdd][x + xAdd] = 1;
					}
				}

				x += 2 * minSpawnSpace;
			}
		}
	}
}

std::optional<std::pair<std::size_t, std::size_t>> Chunk::findObstruction(
    const std::size_t cx, const std::size_t cy, const Terrain& used) const {
	assert(cx - minSpawnSpace >= 0 && cx + minSpawnSpace < used.getXSize() &&
	       cy - minSpawnSpace >= 0 && cy + minSpawnSpace < used.getYSize() &&
	       "Cannot evaluate spawn that goes outside of chunk bounds.");

	// Check from right to left, because if there is something blocking further to the right
	// and we move to avoid something to the left, the next check will anyways be negative.
	for (int x = minSpawnSpace - 1; x > -minSpawnSpace; x--) {
		for (int y = spawnCircleY[std::abs(x)]; y > 0; y--) {
			// Check from top and bottom moving inwards.
			if (used.map[cy - y][cx + x]) return std::make_pair(cx + x, cy - y);
			if (used.map[cy + y][cx + x]) return std::make_pair(cx + x, cy + y);
		}
		// Check for y = 0 (not covered in for loop to avoid double checking).
		if (used.map[cy][cx + x]) return std::make_pair(cx + x, cy);
	}

	return std::nullopt;
}
