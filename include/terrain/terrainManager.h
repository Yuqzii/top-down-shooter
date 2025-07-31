#pragma once

#include <vector>

#include "SDL2/SDL_pixels.h"
#include "engine/Tree2D.h"
#include "engine/game.h"
#include "terrain/terrain.h"

struct SDL_Renderer;
class Scene;
class TerrainCollider;
class Camera;
class Chunk;

class TerrainManager {
public:
	TerrainManager(const Terrain& terrain, const int chunkSize, const int pixelSizeMultiplier,
				   const SDL_Color& color, Scene& scene);

	void updateRender();
	void updateColliders();
	void render(SDL_Renderer* renderer, const Camera& cam) const;

	void setCell(const Vec2& position, const unsigned char value);
	void setCell(const std::pair<int, int>& position, const unsigned char value);
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

	std::size_t getChunksX() const { return chunks[0].size(); }
	std::size_t getChunksY() const { return chunks.size(); }
	int getPixelSize() const { return pixelSize; }
	const Tree2D& getTree() const { return terrainTree; }
	Scene& getScene() const { return scene; }

private:
	Scene& scene;

	int pixelSize;

	const int chunkSize;
	const size_t terrainXSize;
	const size_t terrainYSize;
	std::vector<std::vector<std::unique_ptr<Chunk>>> chunks;
	std::vector<std::vector<std::unique_ptr<Chunk>>> splitToChunks(const Terrain& terrain,
																   const int chunkSize);
	std::pair<std::size_t, std::size_t> posToChunk(
		const std::pair<std::size_t, std::size_t>& pos) const;


	std::vector<GameObject*> terrainColliders;
	Tree2D terrainTree;
	void updateTree();

	SDL_Color color;
};
