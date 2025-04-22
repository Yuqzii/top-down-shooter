#pragma once

#include <map>
#include <vector>

#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "engine/Tree2D.h"
#include "engine/game.h"

struct SDL_Renderer;
class Scene;
class TerrainCollider;

class TerrainManager {
public:
	TerrainManager(const std::vector<std::vector<char>>& terrainMap, const SDL_Color& color,
				   Scene& scene);

	void updateRender();
	void updateCollisions();
	void render(SDL_Renderer* renderer) const;

	void removePixel(const vector2Df& position);
	/*
	 * @abstract	Removes all pixels in range of the center and recalculates collisions.
	 * @param	center	Center position to remove from.
	 * @param	range	The range to remove from. (Radius of circle).
	 */
	void removeInRange(const vector2Df& center, const int range);

	const Tree2D& getTree() const { return terrainTree; }

private:
	Scene& scene;

	std::vector<std::vector<char>> terrainMap;
	const size_t xSize, ySize;
	constexpr static const int pixelSizeMultiplier = 5;
	constexpr static const int pixelSize = Game::pixelSize * pixelSizeMultiplier;

	std::vector<std::vector<SDL_Rect>> renderRects;

	std::vector<GameObject*> terrainColliders;
	Tree2D terrainTree;
	void updateTree();
	/*
	 * @abstract	Creates a TerrainCollider at the middle point between start and end,
	 *			with a line collider from start to end.
	 * @param	start	Start position of LineCollider.
	 * @param	end		End position of LineCollider.
	 * @param	scene	The scene to create the collider in.
	 */
	void createCollider(const vector2Df& start, const vector2Df& end);
	/*
	 * @abstract	Tries to extend an existing collider that ends at start to ending at end.
	 * @param	start	Start position of new collider, used to check against existing ends.
	 * @param	end		End position of new collider, existing collider is extended to this.
	 * @param	currentColliders	Map of existing colliders. Key: end, Value: start.
	 * @param	scene	The scene to create the colliders in.
	 */
	void tryExtendCollider(const std::pair<int, int>& start, const std::pair<int, int>& end,
						   std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders);

	/*
	 * @abstract	Get the array indices of the terrain pixel at the given world position.
	 * @param	position	Position to translate to indices.
	 * @return	Array indices of the world position.
	 */
	std::pair<int, int> posToTerrainCoord(const vector2Df& position) const;

	SDL_Color color;
};
