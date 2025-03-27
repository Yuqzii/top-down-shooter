#include "game/UI/background.h"

namespace UI {

Background::Background(const vector2Df& position, const vector2Df& size, Widget* parent) :
		Widget(parent) {

	// Set variables
	localPosition = position;
	this->size = size;
	localSize = size;

	// Make sure initial positions are correct
	calculatePosition();
	calculateSize();
}

Background::Background(const vector2Df& position, const vector2Df& size) :
		Background(position, size, nullptr) {
}

void Background::update() {
	Widget::update();
}

void Background::render(SDL_Renderer* renderer) const {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);

	Widget::render(renderer);
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
