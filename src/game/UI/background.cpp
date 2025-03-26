#include "game/UI/background.h"

namespace UI {

Background::Background(vector2Df position, vector2Df size) {
	rect.x = position.x;
	rect.y = position.y;
	rect.w = size.x;
	rect.h = size.y;
}

void Background::render(SDL_Renderer* renderer) const {
	SDL_RenderDrawRect(renderer, &rect);
}

}
