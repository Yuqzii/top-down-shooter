#include "terrain/terrainCollider.h"

#include <cmath>

#include "bullet.h"
#include "enemies/enemy.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/chunk.h"
#include "terrain/chunkManager.h"

constexpr float collisionCheckRadius = 500.0f;

TerrainCollider::TerrainCollider(Vec2&& position, Vec2&& start, Vec2&& end, Chunk& chunk)
    : chunk{chunk},
      LineCollider{Collision::Line{std::move(position), std::move(start), std::move(end)},
                   collisionCheckRadius},
#ifdef DEBUG_GIZMO
      fakeObject{std::make_unique<GameObject>()}
#else
      fakeObject{nullptr}
#endif
{
}

void TerrainCollider::update(Scene& scene) {
	LineCollider::checkCollisions(scene);

#ifdef DEBUG_GIZMO
	scene.getGame().getRenderManager().addRenderCall(
	    [this](Scene& scene) {
		    const Vec2& camPos = scene.getCam().getPos();
		    SDL_SetRenderDrawColor(scene.getGame().getRenderer(), 0, 255, 255, 255);
		    SDL_RenderDrawLine(scene.getGame().getRenderer(), line.start.x - camPos.x,
		                       line.start.y - camPos.y, line.end.x - camPos.x,
		                       line.end.y - camPos.y);
	    },
	    fakeObject.get());
#endif
}

void TerrainCollider::onCollision(const Collision::Event& event, Scene& scene) {
	const Bullet* bullet = dynamic_cast<const Bullet*>(event.other->getParent());
	if (bullet) {
		Vec2 newPos = event.position + bullet->getDirection() * 0.1f;
		const auto [nx, ny] = chunk.getManager().posToTerrainCoord(newPos);
		const auto [ox, oy] = chunk.getManager().posToTerrainCoord(event.position);
		if (nx != ox && ny != oy) {
			// newPos is beyond where it should be able to hit,
			// so we choose the one closest to the original collision position.
			const int rx = std::round(newPos.x);
			const int ry = std::round(newPos.y);
			if (std::abs(rx - newPos.x) > std::abs(ry - newPos.y))
				chunk.getManager().changeTerrain(ox, ny, 0);
			else
				chunk.getManager().changeTerrain(nx, oy, 0);
		} else
			chunk.getManager().changeTerrain(nx, ny, 0);
	}
}
