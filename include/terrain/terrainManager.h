#pragma once

#include <vector>
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"

class SDL_Renderer;
class Scene;
class TerrainCollider;

class TerrainManager {
public:
	TerrainManager(const std::vector<std::vector<char>>& terrainMap, const SDL_Color& color);

	void update();
	void updateCollisions(Scene& scene);
	void render(SDL_Renderer* renderer) const;

private:
	std::vector<std::vector<char>> terrainMap;
	const size_t xSize, ySize;
	constexpr static const int pixelSizeMultiplier = 5;

	std::vector<std::vector<SDL_Rect>> renderRects;

	std::vector<TerrainCollider*> terrainColliders;

	SDL_Color color;
};
