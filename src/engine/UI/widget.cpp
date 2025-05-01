#include "engine/UI/widget.h"

namespace UI {

Widget::Widget(Widget* parent, AnchorType anchorPosition) {
	this->parent = parent;

	if (parent != nullptr) parent->addChild(this);

	this->anchorPosition = anchorPosition;
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

// Creates a lambda of the render() function and returns it
// To be used when adding render call to UIManager
std::function<void(SDL_Renderer*)> Widget::getRenderFunction() const {
	return [this](SDL_Renderer* renderer) { this->render(renderer); };
}

void Widget::calculatePosition(const bool& calculateChildren) {
	// No need to calculate position if widget does not have a parent
	if (parent == nullptr) {
		position = localPosition;
	} else {
		// Calculate parents position without updating its children to avoid infinite recursion
		parent->calculatePosition(false);

		// Update position based on anchoring
		Vec2 add;
		switch (anchorPosition.x) {
			case XAnchorType::XANCHOR_CENTER:
				add.x = 50 - (size.x / parent->size.x) * 50;
				break;
			case XAnchorType::XANCHOR_RIGHT:
				add.x = 100 - (size.x / parent->size.x) * 100;
				break;
			default:
				break;
		}

		switch (anchorPosition.y) {
			case YAnchorType::YANCHOR_MIDDLE:
				add.y = 50 - (size.y / parent->size.y) * 50;
				break;
			case YAnchorType::YANCHOR_BOTTOM:
				add.y = 100 - (size.y / parent->size.y) * 100;
				break;
			default:
				break;
		}

		// Update position
		position.x = parent->position.x + parent->size.x * (localPosition.x + add.x) / 100;
		position.y = parent->position.y + parent->size.y * (localPosition.y + add.y) / 100;
	}

	if (calculateChildren) {
		for (auto& child : childWidgets) {
			child->calculatePosition();
		}
	}
}

void Widget::calculateSize() {
	// No need to calculate size if widget does not have a parent
	if (parent == nullptr) {
		size = localSize;
		return;
	}

	parent->calculateSize();  // Make sure parents size is also correct

	size.x = parent->size.x * localSize.x / 100;
	size.y = parent->size.y * localSize.y / 100;
}

void Widget::addChild(Widget* child) { childWidgets.push_back(std::unique_ptr<Widget>(child)); }

}  // namespace UI
