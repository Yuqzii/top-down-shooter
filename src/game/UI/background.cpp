#include "game/UI/background.h"
#include <SDL2/SDL_render.h>

namespace UI {

Background::Background(const vector2Df& position, const vector2Df& size, const SDL_Color& color,
						AnchorType anchorPosition, Widget* parent)
						: Widget(anchorPosition, parent) {
	// Set variables
	localPosition = position;
	this->size = size;
	localSize = size;
	this->color = color;

	// Make sure initial positions are correct
	calculatePosition();
	calculateSize();
}

void Background::update() {
	Widget::update();
}

void Background::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);

	Widget::render(renderer);
}

std::function<void(SDL_Renderer*)> Background::getRenderFunction() const {
	return [this](SDL_Renderer* renderer){this->render(renderer);};
}

void Background::calculatePosition() {
	Widget::calculatePosition();

	rect.x = position.x;
	rect.y = position.y;
}

void Background::calculateSize() {
	Widget::calculateSize();

	rect.w = size.x;
	rect.h = size.y;
}

}
