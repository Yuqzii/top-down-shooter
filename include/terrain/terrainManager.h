#pragma once

#include <vector>

#include "SDL2/SDL_pixels.h"
#include "engine/Tree2D.h"
#include "engine/game.h"
#include "terrain/chunk.h"
#include "terrain/terrain.h"

struct SDL_Renderer;
class Scene;
class TerrainCollider;
class Camera;

class TerrainManager {
public:
	TerrainManager(const Terrain& terrain, const std::size_t chunkSize,
	               const int pixelSizeMultiplier, const SDL_Color& color, Scene& scene);

	void update(const Vec2& playerPos);
	void collisionUpdate();

	void updateRender();
	void updateColliders();
	void render(SDL_Renderer* renderer, const Camera& cam, const Vec2& playerPos) const;

	void setCell(const Vec2& position, const unsigned char value);
	void setCell(const std::pair<std::size_t, std::size_t>& position, const unsigned char value);
	void setCell(const std::size_t x, const std::size_t y, const unsigned char value);
	/* Removes all pixels in range of the center and recalculates collisions.
	 *
	 * @param center Center position to remove from.
	 * @param range The range to remove from. (Radius of circle).
	 */
	void setCellsInRange(const Vec2& center, int range, const unsigned char value);
	/* Get the array indices of the terrain pixel at the given world position.
	 *
	 * @param position Position to translate to indices.
	 * @return Array indices of the world position.
	 */
	std::pair<std::size_t, std::size_t> posToTerrainCoord(const Vec2& position) const;

	std::size_t getChunksX() const { return chunks.empty() ? 0 : chunks[0].size(); }
	std::size_t getChunksY() const { return chunks.size(); }
	std::size_t getChunkSize() const { return chunkSize; }
	int getPixelSize() const { return pixelSize; }
	// DEPRECATED, does not return a correct tree.
	const Tree2D& getTree() const { return terrainTree; }
	Scene& getScene() const { return scene; }

	std::vector<Vec2> getAllSpawns() const;

private:
	Scene& scene;

	int pixelSize;

	const std::size_t chunkSize;
	const std::size_t terrainXSize;
	const std::size_t terrainYSize;
	std::vector<std::vector<Chunk>> chunks;
	std::vector<std::vector<Chunk>> splitToChunks(const Terrain& terrain,
	                                              const std::size_t chunkSize);
	std::pair<std::size_t, std::size_t> posToChunk(
	    const std::pair<std::size_t, std::size_t>& pos) const;
	/* @param x Center X-position in chunk coordinates.
	 * @param y Center Y-position in chunk coordinates.
	 */
	std::vector<std::reference_wrapper<Chunk>> getChunksInRange(const std::size_t x,
	                                                            const std::size_t y,
	                                                            const int range);
	std::vector<std::reference_wrapper<const Chunk>> getConstChunksInRange(const std::size_t x,
	                                                                       const std::size_t y,
	                                                                       const int range) const;

	// DEPRECATED. TerrainManager does not know about all it's terrainColliders.
	std::vector<GameObject*> terrainColliders;
	Tree2D terrainTree;
	void updateTree() {}

	SDL_Color color;
};
