#include "game/UI/widget.h"

namespace UI {

Widget::Widget() {
	canvas = nullptr;
}

Widget::Widget(Canvas* canvas) {
	this->canvas = canvas;
}

void Widget::update() {
	
}

void Widget::render(SDL_Renderer* renderer) const {
	// Widget is to be rendered based on absolute screen position
	if (canvas == nullptr) {
		
	}
}

}
