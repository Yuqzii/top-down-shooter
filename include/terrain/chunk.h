#pragma once

#include <cstddef>
#include <map>

#include "SDL2/SDL_rect.h"
#include "terrain/terrain.h"

class Scene;
class TerrainManager;

class Chunk {
public:
	Chunk(const std::vector<std::vector<unsigned char>>& map, TerrainManager& manager);

	/* Sets the cell at position (x, y) to value.
	 * x and y position is relative to this chunk.
	 */
	void setCell(const std::size_t x, const std::size_t y, const unsigned char value);

	void updateRender(const int pixelSize);
	void updateColliders();

private:
	TerrainManager& manager;

	Terrain terrain;

	std::vector<std::vector<SDL_Rect>> renderRects;

	/* Tries to extend an existing collider that ends at start to ending at end.
	 *
	 * @param start Start position of new collider, used to check against existing ends.
	 * @param end End position of new collider, existing collider is extended to this.
	 * @param currentColliders Map of existing colliders. Key: end, Value: start.
	 * @param scene The scene to create the colliders in.
	 */
	void tryExtendCollider(const std::pair<int, int>& start, const std::pair<int, int>& end,
						   std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders);
};
