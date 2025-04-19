#include "terrain/terrainCollider.h"
#include "engine/scene.h"
#include "engine/game.h"

TerrainCollider::TerrainCollider() : GameObject{vector2Df{}} {
	collider = std::make_unique<LineCollider>(Collision::Line{}, 300.0f, this);
	isStatic = true;
}

void TerrainCollider::initializeCollider(const vector2Df& start, const vector2Df& end) {
	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	lineCollider->line.start = start;
	lineCollider->line.end = end;
}

void TerrainCollider::update(Scene& scene, const float deltaTime) {
	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	const vector2Df dir = vector2Df(lineCollider->line.end - lineCollider->line.start).normalized();
	normal = vector2Df(dir.y, dir.x * -1.0f) * 25.0f;
	//normal = dir.normalized() * 25.0f;
	scene.getGame().getRenderManager().addRenderCall([this](SDL_Renderer* renderer) {
		SDL_RenderDrawLine(renderer, position.x, position.y, position.x + normal.x, position.y + normal.y);
	}, this);
}
