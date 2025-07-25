#pragma once

#include <vector>

struct Terrain {
	std::vector<std::vector<unsigned char>> map;

	size_t getXSize() const { return map[0].size(); }
	size_t getYSize() const { return map.size(); }

	Terrain(size_t xSize, size_t ySize) { map.resize(ySize, std::vector<unsigned char>(xSize)); }
	Terrain(const std::vector<std::vector<unsigned char>>& map) : map{map} {}
};
