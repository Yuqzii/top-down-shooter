#include "engine/UI/slider.h"

#include <algorithm>

namespace UI {

Slider::Slider(const SDL_Color& color, Widget* parent)
    : Background(Vec2(), Vec2(100, 100), color, parent) {
	sliderValue = 100;
}

void Slider::setValue(const float& newValue) {
	sliderValue = std::clamp(newValue, 0.0f, 100.0f);  // Clamp sliderValue between 0 and 100
	localSize.x = sliderValue;
	calculateSize();
}

}  // namespace UI
