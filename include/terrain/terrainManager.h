#pragma once

#include <vector>

class SDL_Renderer;

class TerrainManager {
public:
	TerrainManager(const std::vector<std::vector<char>>& terrainMap);


	void render(SDL_Renderer* renderer) const;

private:
	std::vector<std::vector<char>> terrainMap;
	const size_t xSize, ySize;
};
