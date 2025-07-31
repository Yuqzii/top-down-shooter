#include "terrain/terrainCollider.h"

#include <cmath>

#include "bullet.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/chunk.h"
#include "terrain/terrainManager.h"

TerrainCollider::TerrainCollider() : GameObject{Vec2{}} {
	// Create a static line collider and set this GameObject as static
	collider = std::make_unique<LineCollider>(Collision::Line{}, true, this);
	isStatic = true;
	renderObject = false;
}

void TerrainCollider::initialize(const Scene& scene, const Vec2& position, const Vec2& start,
								 const Vec2& end, Chunk&& chunk) {
	GameObject::initialize(scene, position);

	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	lineCollider->line.start = start;
	lineCollider->line.end = end;

	this->chunk = &chunk;
}

void TerrainCollider::onCollision(const Collision::Event& event, Scene& scene) {
	const Bullet* bullet = dynamic_cast<const Bullet*>(event.other->getParent());
	if (bullet) {
		Vec2 newPos = event.position + bullet->getDirection() * 0.1f;
		const auto [nx, ny] = chunk->getManager().posToTerrainCoord(newPos);
		const auto [ox, oy] = chunk->getManager().posToTerrainCoord(event.position);
		if (nx != ox && ny != oy) {
			// newPos is beyond where it should be able to hit,
			// so we choose the one closest to the original collision position.
			const int rx = std::round(newPos.x);
			const int ry = std::round(newPos.y);
			if (std::abs(rx - newPos.x) > std::abs(ry - newPos.y))
				chunk->getManager().setCell(ox, ny, 0);
			else
				chunk->getManager().setCell(nx, oy, 0);
		} else
			chunk->getManager().setCell(nx, ny, 0);
	}
}

// ONLY USED FOR DEBUG GIZMOS
void TerrainCollider::update(Scene& scene, const float deltaTime) {
#ifndef DEBUG_GIZMO
	return;
#endif
	GameObject::update(scene, deltaTime);

	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	const Vec2 dir = Vec2(lineCollider->line.end - lineCollider->line.start).normalized();
	normal = Vec2(dir.y, dir.x * -1.0f) * 25.0f;
	scene.getGame().getRenderManager().addRenderCall(
		[this](Scene& scene) {
			const Vec2 camPos = scene.getCam().getPos();
			SDL_RenderDrawLine(scene.getGame().getRenderer(), position.x - camPos.x,
							   position.y - camPos.y, position.x - camPos.x + normal.x,
							   position.y - camPos.y + normal.y);
		},
		this);
}
