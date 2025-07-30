#include "terrain/chunk.h"

#include <cassert>

Chunk::Chunk(const std::vector<std::vector<unsigned char>>& map) : terrain{map} {}

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
