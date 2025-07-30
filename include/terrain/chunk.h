#pragma once

#include <cstddef>

#include "SDL2/SDL_rect.h"
#include "terrain/terrain.h"

class Chunk {
public:
	Chunk(const std::vector<std::vector<unsigned char>>& map);

	/* Sets the cell at position (x, y) to value.
	 * x and y position is relative to this chunk.
	 */
	void setCell(const std::size_t x, const std::size_t y, const unsigned char value);

	void updateRender(const int pixelSize);
	void updateCollisions();

private:
	Terrain terrain;

	std::vector<std::vector<SDL_Rect>> renderRects;
};
