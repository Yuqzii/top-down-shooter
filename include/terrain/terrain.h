#pragma once

#include <vector>

struct Terrain {
	std::vector<std::vector<unsigned char>> map;

	Terrain(const size_t xSize, const size_t ySize);
	Terrain(const std::vector<std::vector<unsigned char>>& map);

	std::size_t getXSize() const { return map.empty() ? 0 : map[0].size(); }
	std::size_t getYSize() const { return map.size(); }

	void printTerrain() const;
};
