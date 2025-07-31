#pragma once

#include <cstddef>
#include <map>

#include "SDL2/SDL_rect.h"
#include "terrain/terrain.h"

class Scene;
class TerrainManager;
class TerrainCollider;
struct Vec2;
struct SDL_Renderer;
class Camera;

class Chunk {
public:
	Chunk(const std::vector<std::vector<unsigned char>>& map, const std::size_t originX, const std::size_t originY, TerrainManager& manager);

	/* Sets the cell at position (x, y) to value.
	 * x and y position is relative to this chunk.
	 */
	void setCell(const std::size_t x, const std::size_t y, const unsigned char value);
	void setCellMultiple(const std::vector<std::pair<size_t, size_t>>& positions,
						 const unsigned char value);

	void render(SDL_Renderer* renderer, const Camera& cam) const;
	void updateRender(const int pixelSize);
	void updateColliders();

	const TerrainManager& getManager() const { return manager; }
	const Terrain& getTerrain() const { return terrain; }

private:
	const TerrainManager& manager;

	Terrain terrain;
	const std::size_t originX;
	const std::size_t originY;

	std::vector<std::vector<SDL_Rect>> renderRects;
	std::vector<std::reference_wrapper<TerrainCollider>> colliders;

	/* Tries to extend an existing collider that ends at start to ending at end.
	 *
	 * @param start Start position of new collider, used to check against existing ends.
	 * @param end End position of new collider, existing collider is extended to this.
	 * @param currentColliders Map of existing colliders. Key: end, Value: start.
	 * @param scene The scene to create the colliders in.
	 */
	void tryExtendCollider(const std::pair<int, int>& start, const std::pair<int, int>& end,
						   std::map<std::pair<int, int>, std::pair<int, int>>& currentColliders);
	/* Creates a TerrainCollider at the middle point between start and end,
	 * with a line collider from start to end.
	 *
	 * @param start Start position of LineCollider.
	 * @param end End position of LineCollider.
	 * @param scene The scene to create the collider in.
	 */
	void createCollider(const Vec2& start, const Vec2& end);
};
