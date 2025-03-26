#include "game/UI/canvas.h"

namespace UI {

void Canvas::update() {
	for (auto it = widgets.begin(); it != widgets.end(); it++) {
		it->update();
	}
}

void Canvas::render(SDL_Renderer* renderer) const {
	for (auto it = widgets.cbegin(); it != widgets.cend(); it++) {
		it->render(renderer);
	}
}

}
