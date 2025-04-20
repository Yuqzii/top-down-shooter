#include "terrain/terrainManager.h"

#include <map>

#include "SDL2/SDL_render.h"
#include "engine/game.h"
#include "terrain/terrainCollider.h"
#include "engine/scene.h"

TerrainManager::TerrainManager(const std::vector<std::vector<char>>& terrainMap_,
							   const SDL_Color& color_)
	: terrainMap{terrainMap_}, xSize{terrainMap.size()}, ySize{terrainMap[0].size()},
	  color(color_) {
	renderRects.resize(xSize);
	for (auto& rectList : renderRects)
		rectList.resize(ySize);

	update();
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

void TerrainManager::updateCollisions(Scene& scene) {
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
				tryExtendCollider(topLeft, botLeft, currentColliders, scene);
			}
			else if (left && !right && above && below) {
				// Only empty to the right
				tryExtendCollider(topRight, botRight, currentColliders, scene);
			}
			else if (left && right && !above && below) {
				// Only empty above
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
			}
			else if (left && right && above && !below) {
				// Only empty below
				tryExtendCollider(botLeft, botRight, currentColliders, scene);
			}
			else if (!left && !right && above && below) {
				// Straight vertical line
				tryExtendCollider(topLeft, botLeft, currentColliders, scene);
				tryExtendCollider(topRight, botRight, currentColliders, scene);
			}
			else if ((!left && right && !above && below) || (left && !right && above && !below)) {
				// Diagonal line from top right to bottom left
				tryExtendCollider(topRight, botLeft, currentColliders, scene);
			}
			else if ((!left && right && above && !below) || (left && !right && !above && below)) {
				// Diagonal from top left to bottom right
				tryExtendCollider(topLeft, botRight, currentColliders, scene);
			}
			else if (left && right && !above && !below) {
				// Straight horizontal line
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
				tryExtendCollider(botLeft, botRight, currentColliders, scene);
			}
			else if (!left && !right && !above && below) {
				// Three lines, vertical left, horizontal above, and vertical right
				tryExtendCollider(topLeft, botLeft, currentColliders, scene);
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
				tryExtendCollider(topRight, botRight, currentColliders, scene);
			}
			else if (!left && !right && above && !below) {
				// Three line, vertical left, vertical right, and horizontal below
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
				tryExtendCollider(topRight, botRight, currentColliders, scene);
				tryExtendCollider(botLeft, botRight, currentColliders, scene);
			}
			else if (!left && right && !above && !below) {
				// Three lines, vertical left, horizontal above, and horizontal below
				tryExtendCollider(topLeft, botLeft, currentColliders, scene);
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
				tryExtendCollider(botLeft, botRight, currentColliders, scene);
			}
			else if (left && !right && !above && !below) {
				// Three lines, vertical right, horizontal above, and horizontal below
				tryExtendCollider(topRight, botRight, currentColliders, scene);
				tryExtendCollider(topLeft, topRight, currentColliders, scene);
				tryExtendCollider(botLeft, botRight, currentColliders, scene);
			}
		}
	}

	// Construct colliders
	for (const auto& [end, start] : currentColliders) {
		createCollider(vector2Df{start.first, start.second},
					   vector2Df{end.first, end.second}, scene);
	}

	updateTree();
}

void TerrainManager::tryExtendCollider(const std::pair<int, int>& start,
									   const std::pair<int, int>& end,
									   std::map<std::pair<int, int>, std::pair<int, int>>&
									   currentColliders, Scene& scene) {
		const vector2Df startVec{start.first, start.second};
		const vector2Df endVec{end.first, end.second};

		if (currentColliders.count(end)) {
			createCollider(vector2Df{currentColliders[end].first, currentColliders[end].second},
				  endVec, scene);
			currentColliders.erase(end);
		}

		if (currentColliders.count(start)){
			const vector2Df curStartVec{currentColliders[start].first,
										currentColliders[start].second};

			if ((startVec - curStartVec).normalized() == (endVec - startVec).normalized()) {
				auto node = currentColliders.extract(start);
				node.key() = end;
				currentColliders.insert(std::move(node));
				return;
			}
		}
		currentColliders[end] = start;
}

void TerrainManager::createCollider(const vector2Df& start, const vector2Df& end, Scene& scene) {
		const vector2Df position{start + (end - start) * 0.5f};
		TerrainCollider& collider = scene.instantiate<TerrainCollider>(position);
		collider.initializeCollider(start, end);
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
