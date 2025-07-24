#pragma once

#include <vector>

struct Terrain {
	std::vector<std::vector<unsigned char>> map;

	Terrain(size_t xSize, size_t ySize) { map.resize(ySize, std::vector<unsigned char>(xSize)); }
	Terrain(const std::vector<std::vector<unsigned char>>& map) : map{map} {}
};
