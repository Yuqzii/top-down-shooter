#pragma once

#include <vector>
#include "SDL2/SDL_pixels.h"

class SDL_Renderer;

class TerrainManager {
public:
	TerrainManager(const std::vector<std::vector<char>>& terrainMap, const SDL_Color& color);

	void render(SDL_Renderer* renderer) const;

private:
	std::vector<std::vector<char>> terrainMap;
	const size_t xSize, ySize;

	SDL_Color color;
};
