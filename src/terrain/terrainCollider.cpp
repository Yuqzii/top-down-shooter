#include "terrain/terrainCollider.h"

#include "bullet.h"
#include "engine/game.h"
#include "engine/scene.h"
#include "terrain/terrainManager.h"

TerrainCollider::TerrainCollider() : GameObject{Vec2{}} {
	// Create a static line collider and set this GameObject as static
	collider = std::make_unique<LineCollider>(Collision::Line{}, true, this);
	isStatic = true;
	renderObject = false;
}

void TerrainCollider::initialize(const Scene& scene, const Vec2& position, const Vec2& start,
								 const Vec2& end, TerrainManager* manager) {
	GameObject::initialize(scene, position);

	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	lineCollider->line.start = start;
	lineCollider->line.end = end;
	this->manager = manager;
}

void TerrainCollider::onCollision(const Collision::Event& event, Scene& scene) {
	const Bullet* bullet = dynamic_cast<const Bullet*>(event.other->getParent());
	if (bullet) {
		// Remove pixel at collision position
		manager->removePixel(event.position + bullet->getDirection() * 2.0f);
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
