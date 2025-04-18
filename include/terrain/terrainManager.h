#pragma once

#include <map>
#include <vector>
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "engine/game.h"

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
	constexpr static const int pixelSize = Game::pixelSize * pixelSizeMultiplier;

	std::vector<std::vector<SDL_Rect>> renderRects;

	std::vector<TerrainCollider*> terrainColliders;
	/*
	* @abstract	Creates a TerrainCollider at the middle point between start and end,
	*			with a line collider from start to end.
	* @param	start	Start position of LineCollider.
	* @param	end		End position of LineCollider.
	* @param	scene	The scene to create the collider in.
	*/
	void createCollider(const vector2Df& start, const vector2Df& end, Scene& scene);
	/*
	* @abstract	Tries to extend an existing collider that ends at start to ending at end.
	* @param	start	Start position of new collider, used to check against existing ends.
	* @param	end		End position of new collider, existing collider is extended to this.
	* @param	currentColliders	Map of existing colliders. Key: end, Value: start.
	* @param	scene	The scene to create the colliders in.
	*/
	void tryExtendCollider(const std::pair<int, int>& start, const std::pair<int, int>& end,
						   std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders,
						   Scene& scene);

	SDL_Color color;
};
