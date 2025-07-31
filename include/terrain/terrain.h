#pragma once

#include <vector>

struct Terrain {
	std::vector<std::vector<unsigned char>> map;

	Terrain(const size_t xSize, const size_t ySize);
	Terrain(const std::vector<std::vector<unsigned char>>& map);

	size_t getXSize() const { return map[0].size(); }
	size_t getYSize() const { return map.size(); }

	void printTerrain() const;
};
