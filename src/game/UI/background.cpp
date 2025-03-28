#include "game/UI/background.h"
#include <SDL2/SDL_render.h>

namespace UI {

Background::Background(const vector2Df& position, const vector2Df& size, const SDL_Color& color,
						Widget* parent, AnchorType anchorPosition)
						: Widget(parent, anchorPosition) {
	// Set variables
	localPosition = position;
	localSize = size;
	this->color = color;

	// Make sure initial positions are correct
	calculatePosition();
	calculateSize();
}

void Background::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);

	Widget::render(renderer);
}

void Background::calculatePosition(const bool& calculateChildren) {
	Widget::calculatePosition(calculateChildren);

	rect.x = position.x;
	rect.y = position.y;
}

void Background::calculateSize() {
	Widget::calculateSize();

	rect.w = size.x;
	rect.h = size.y;
}

}
