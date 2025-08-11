#pragma once

#include <array>
#include <cstddef>
#include <map>
#include <optional>

#include "SDL2/SDL_rect.h"
#include "terrain/terrain.h"
#include "terrain/terrainCollider.h"

class Scene;
struct Vec2;
struct SDL_Renderer;
class Camera;
class TerrainChange;
class TerrainManager;

class Chunk {
public:
	Chunk(std::vector<std::vector<unsigned char>>&& map, const std::size_t originX,
	      const std::size_t originY, TerrainManager& manager);

	/* Sets the cell at position (x, y) to value.
	 * x and y position is relative to this chunk.
	 */
	void changeTerrain(const TerrainChange& change);
	void changeTerrainMultiple(const std::vector<TerrainChange>& changes);

	void update(Scene& scene);
	void collisionUpdate(Scene& scene);

	void render(SDL_Renderer* renderer, const Camera& cam) const;
	void updateRender(const int pixelSize);

	void updateColliders();
	std::size_t getColliderCount() const { return colliders.size(); }

	void updateSpawnPositions();

	TerrainManager& getManager() const { return manager; }
	const Terrain& getTerrain() const { return terrain; }
	const std::vector<Vec2>& getSpawnPositions() const { return spawnPositions; }

private:
	TerrainManager& manager;

	Terrain terrain;
	const std::size_t originX;
	const std::size_t originY;

	std::vector<std::vector<SDL_Rect>> renderRects;
	std::vector<TerrainCollider> colliders;

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
	void createCollider(Vec2&& start, Vec2&& end);

	static constexpr int minSpawnSpace = 15;
	static std::array<int, minSpawnSpace> spawnCircleY;
	std::vector<Vec2> spawnPositions;
	// @return Position where there is terrain blocking. Has no value if none were found.
	std::optional<std::pair<std::size_t, std::size_t>> findObstruction(const std::size_t x,
	                                                                   const std::size_t y,
	                                                                   const Terrain& used) const;
};
