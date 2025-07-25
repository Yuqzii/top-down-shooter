#pragma once

#include <map>
#include <vector>

#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "engine/Tree2D.h"
#include "engine/game.h"
#include "terrain/terrain.h"

struct SDL_Renderer;
class Scene;
class TerrainCollider;
class Camera;

class TerrainManager {
public:
	// Must be initialized with a non-empty terrainMap vector
	TerrainManager(const Terrain& terrain, const SDL_Color& color, Scene& scene);

	void updateRender();
	void updateCollisions();
	void render(SDL_Renderer* renderer, const Camera& cam) const;

	void removePixel(const Vec2& position);
	void removePixel(const std::pair<int, int>& position);
	/*
	 * @abstract	Removes all pixels in range of the center and recalculates collisions.
	 * @param	center	Center position to remove from.
	 * @param	range	The range to remove from. (Radius of circle).
	 */
	void removeInRange(const Vec2& center, const int range);
	/*
	 * @abstract	Get the array indices of the terrain pixel at the given world position.
	 * @param	position	Position to translate to indices.
	 * @return	Array indices of the world position.
	 */
	std::pair<int, int> posToTerrainCoord(const Vec2& position) const;

	const Tree2D& getTree() const { return terrainTree; }

private:
	Scene& scene;

	Terrain terrain;
	const size_t xSize, ySize;
	constexpr static const int pixelSizeMultiplier = 3;
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
	void createCollider(const Vec2& start, const Vec2& end);
	/*
	 * @abstract	Tries to extend an existing collider that ends at start to ending at end.
	 * @param	start	Start position of new collider, used to check against existing ends.
	 * @param	end		End position of new collider, existing collider is extended to this.
	 * @param	currentColliders	Map of existing colliders. Key: end, Value: start.
	 * @param	scene	The scene to create the colliders in.
	 */
	void tryExtendCollider(const std::pair<int, int>& start, const std::pair<int, int>& end,
						   std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders);

	SDL_Color color;
};
