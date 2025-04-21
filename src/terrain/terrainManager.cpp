#include "terrain/terrainManager.h"

#include <map>

#include "SDL2/SDL_render.h"
#include "engine/game.h"
#include "terrain/terrainCollider.h"
#include "engine/scene.h"

TerrainManager::TerrainManager(const std::vector<std::vector<char>>& terrainMap_,
							   const SDL_Color& color_, Scene& scene_)
	: terrainMap{terrainMap_}, xSize{terrainMap.size()}, ySize{terrainMap[0].size()},
	  color{color_}, scene{scene_} {
	renderRects.resize(xSize);
	for (auto& rectList : renderRects)
		rectList.resize(ySize);
}

void TerrainManager::update() {
	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			// Add render rect if there is terrain at the current position
			if (terrainMap[x][y]) {
				// Does this pixel need recalculation?
				if (renderRects[x][y].w != 0) continue;

				renderRects[x][y].x = x * pixelSize;
				renderRects[x][y].y = y * pixelSize;
				renderRects[x][y].w = renderRects[x][y].h = pixelSize;
			}
			else {
				// Zero rect so that is is not rendered
				renderRects[x][y].w = renderRects[x][y].h = 0;
			}
		}
	}
}

void TerrainManager::updateCollisions() {
	// Remove previous colliders
	for (auto it = terrainColliders.begin(); it != terrainColliders.end();) {
		(*it)->deleteObject = true;
		it = terrainColliders.erase(it);
	}

	std::map<std::pair<int, int>, std::pair<int, int>> currentColliders; // Key: end, Value: start

	for (int x = 0; x < xSize; ++x) {
		const float xPos = x * pixelSize;
		for (int y = 0; y < ySize; ++y) {
			if (!terrainMap[x][y]) continue;

			const float yPos = y * pixelSize;
			const std::pair<int, int> topLeft{xPos, yPos};
			const std::pair<int, int> topRight{xPos + pixelSize, yPos};
			const std::pair<int, int> botLeft{xPos, yPos + pixelSize};
			const std::pair<int, int> botRight{xPos + pixelSize, yPos + pixelSize};

			const bool left = x > 0 && terrainMap[x - 1][y];
			const bool right = x < xSize - 1 && terrainMap[x + 1][y];
			const bool above = y > 0 && terrainMap[x][y - 1];
			const bool below = y < ySize - 1 && terrainMap[x][y + 1];

			if (left && right && above && below)
				continue; // Terrain in all directions
			else if (!left && right && above && below) {
				// Only empty to the left
				tryExtendCollider(topLeft, botLeft, currentColliders);
			}
			else if (left && !right && above && below) {
				// Only empty to the right
				tryExtendCollider(topRight, botRight, currentColliders);
			}
			else if (left && right && !above && below) {
				// Only empty above
				tryExtendCollider(topLeft, topRight, currentColliders);
			}
			else if (left && right && above && !below) {
				// Only empty below
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
			else if (!left && !right && above && below) {
				// Straight vertical line
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
			}
			else if ((!left && right && !above && below) || (left && !right && above && !below)) {
				// Diagonal line from top right to bottom left
				tryExtendCollider(topRight, botLeft, currentColliders);
			}
			else if ((!left && right && above && !below) || (left && !right && !above && below)) {
				// Diagonal from top left to bottom right
				tryExtendCollider(topLeft, botRight, currentColliders);
			}
			else if (left && right && !above && !below) {
				// Straight horizontal line
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
			else if (!left && !right && !above && below) {
				// Three lines, vertical left, horizontal above, and vertical right
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
			}
			else if (!left && !right && above && !below) {
				// Three line, vertical left, vertical right, and horizontal below
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
			else if (!left && right && !above && !below) {
				// Three lines, vertical left, horizontal above, and horizontal below
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
			else if (left && !right && !above && !below) {
				// Three lines, vertical right, horizontal above, and horizontal below
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
			else if (!left && !right && !above && !below) {
				// Collidersr on every side
				tryExtendCollider(topLeft, botLeft, currentColliders);
				tryExtendCollider(topRight, botRight, currentColliders);
				tryExtendCollider(topLeft, topRight, currentColliders);
				tryExtendCollider(botLeft, botRight, currentColliders);
			}
		}
	}

	// Construct colliders
	for (const auto& [end, start] : currentColliders) {
		createCollider(vector2Df{start.first, start.second},
					   vector2Df{end.first, end.second});
	}

	updateTree();
}

void TerrainManager::tryExtendCollider(const std::pair<int, int>& start,
									   const std::pair<int, int>& end,
									   std::map<std::pair<int, int>, std::pair<int, int>>&
									   currentColliders) {
	const vector2Df startVec{start.first, start.second};
	const vector2Df endVec{end.first, end.second};

	// Create collider if there already is one ending at end
	if (currentColliders.count(end)) {
		createCollider(vector2Df{currentColliders[end].first, currentColliders[end].second},
				 endVec);
		currentColliders.erase(end);
	}

	// Is there a collider ending at start?
	if (currentColliders.count(start)){
		const vector2Df curStartVec{currentColliders[start].first,
			currentColliders[start].second};

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

void TerrainManager::createCollider(const vector2Df& start, const vector2Df& end) {
		const vector2Df position{start + (end - start) * 0.5f};
		TerrainCollider& collider = scene.instantiate<TerrainCollider>(position);
		collider.initializeCollider(start, end, *this);
		terrainColliders.push_back(&collider);
}

void TerrainManager::updateTree() {
	terrainTree = Tree2D{};
	terrainTree.initializeWithList(terrainColliders);
}

void TerrainManager::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (auto& rectList : renderRects) {
		SDL_RenderFillRects(renderer, &rectList[0], rectList.size());
	}
}

void TerrainManager::removePixel(const vector2Df& position) {
	const auto [x, y] = posToTerrainCoord(position);
	if (x >= xSize || y >= ySize || !terrainMap[x][y])
		return;

	terrainMap[x][y] = false;
	update();
	updateCollisions();
}

void TerrainManager::removeInRange(const vector2Df& center, int range) {
	++range; // Increment range to make calculations work correctly

	// Find the height (sine) of every x position based on the range (radius)
	std::vector<int> maxY(range);
	for (int x = 0; x < range; ++x) {
		maxY[x] = std::round(std::sin(std::acos((float)x / range)) * range);
	}

	
	const auto [cX, cY] = posToTerrainCoord(center); // Get center to remove from
	// Loop through and deactivate pixels
	for (int x = -range + 1; x < range; ++x) {
		if (cX + x < 0 || cX + x >= xSize) continue;
		for (int y = -maxY[std::abs(x)] + 1; y < maxY[std::abs(x)]; ++y) {
			if (cY + y < 0 || cY + y >= ySize) continue;
			terrainMap[cX + x][cY + y] = false;
		}
	}

	update();
	updateCollisions();
}

std::pair<int, int> TerrainManager::posToTerrainCoord(const vector2Df& position) const {
	const int x = position.x / pixelSize;
	const int y = position.y / pixelSize;
	return std::move(std::pair<int, int>{x, y});
}
