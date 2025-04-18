#include "terrain/terrainCollider.h"

TerrainCollider::TerrainCollider() {
	collider = std::make_unique<LineCollider>(Collision::Line{}, 300.0f, this);
	isStatic = true;
}

void TerrainCollider::initializeCollider(const vector2Df& start, const vector2Df& end) {
	LineCollider* lineCollider = static_cast<LineCollider*>(collider.get());
	lineCollider->line.start = start;
	lineCollider->line.end = end;
}
