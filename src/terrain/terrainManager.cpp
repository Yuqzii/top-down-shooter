#include "terrain/terrainManager.h"
#include "SDL2/SDL_render.h"
#include "engine/game.h"

TerrainManager::TerrainManager(const std::vector<std::vector<char>>& terrainMap_,
							   const SDL_Color& color_)
	: terrainMap{terrainMap_}, xSize{terrainMap.size()}, ySize{terrainMap[0].size()},
	  color(color_) {
	renderRects.resize(xSize);
	for (auto& rectList : renderRects)
		rectList.resize(ySize);

	update();
}

void TerrainManager::update() {
	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			// Add render rect if there is terrain at the current position
			if (terrainMap[x][y]) {
				// Does this pixel need recalculation?
				if (renderRects[x][y].w != 0) continue;

				renderRects[x][y].x = x * Game::pixelSize * pixelSizeMultiplier;
				renderRects[x][y].y = y * Game::pixelSize * pixelSizeMultiplier;
				renderRects[x][y].w = renderRects[x][y].h = Game::pixelSize * pixelSizeMultiplier;
			}
			else {
				// Zero rect so that is is not rendered
				renderRects[x][y].w = renderRects[x][y].h = 0;
			}
		}
	}
}

void TerrainManager::updateCollisions() {
	

	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			
		}
	}
}

void TerrainManager::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	for (auto& rectList : renderRects) {
		SDL_RenderFillRects(renderer, &rectList[0], rectList.size());
	}
}
