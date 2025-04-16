#include "terrain/terrainManager.h"
#include "SDL2/SDL_render.h"
#include "engine/game.h"

TerrainManager::TerrainManager(const std::vector<std::vector<char>>& terrainMap_,
							   const SDL_Color& color_)
	: terrainMap{terrainMap_}, xSize{terrainMap.size()}, ySize{terrainMap[0].size()},
	  color(color_)
{}

void TerrainManager::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (int x = 0; x < xSize; x++) {
		std::vector<SDL_Rect> rects;
		rects.reserve(ySize);
		for (int y = 0; y < ySize; y++) {
			// Add render rect if there is terrain at the current position
			if (terrainMap[x][y]) {
				rects.push_back(SDL_Rect{x * Game::pixelSize * 5, y * Game::pixelSize * 5,
					Game::pixelSize * 5, Game::pixelSize * 5});
			}
		}

		// Draw all the terrain "pixels" for the current x-position
		SDL_RenderFillRects(renderer, &rects[0], rects.size());
	}
}
