#include "game/UI/widget.h"

namespace UI {

Widget::Widget() {
	parent = nullptr;
}

Widget::Widget(Widget* parent) {
	this->parent = parent;

	if (parent != nullptr)
		parent->addChild(this);
}

void Widget::update() {
	for (auto& child : childWidgets) {
		child->update();
	}
}

void Widget::render(SDL_Renderer* renderer) const {
	for (auto& child : childWidgets) {
		child->render(renderer);
	}
}

std::function<void(SDL_Renderer*)> Widget::getRenderFunction() const {
	return [this](SDL_Renderer* renderer){this->render(renderer);};
}

void Widget::calculatePosition() {
	// No need to calculate position if widget does not have a parent
	if (parent == nullptr) {
		position = localPosition;
		return;
	}

	parent->calculatePosition(); // Make sure parents position is also correct
	// Update position
	position.x = parent->position.x + parent->size.x * localPosition.x / 100;
	position.y = parent->position.y + parent->size.y * localPosition.y / 100;
}

void Widget::calculateSize() {
	// No need to calculate size if widget does not have a parent
	if (parent == nullptr) return;

	parent->calculateSize(); // Make sure parents size is also correct

	size.x = parent->size.x * localSize.x / 100;
	size.y = parent->size.y * localSize.y / 100;
}

void Widget::addChild(Widget* child) {
	childWidgets.push_back(std::move(std::unique_ptr<Widget>(child)));
}

}
