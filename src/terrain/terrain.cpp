#include "terrain/terrain.h"

#include <iostream>

Terrain::Terrain(const size_t xSize, const size_t ySize) {
	map.resize(ySize, std::vector<unsigned char>(xSize));
}

Terrain::Terrain(const std::vector<std::vector<unsigned char>>& map) : map{map} {}

void Terrain::printTerrain() const {
	for (size_t y = 0; y < getYSize(); y++) {
		for (size_t x = 0; x < getXSize(); x++) std::cout << (map[y][x] ? '#' : '.');
		std::cout << '\n';
	}
	std::cout << '\n';
}
