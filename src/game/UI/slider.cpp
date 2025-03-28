#include "game/UI/slider.h"

namespace UI {

Slider::Slider(const SDL_Color& color, Widget* parent) :
		Background(vector2Df(), vector2Df(100, 100), color, parent) {
	sliderValue = 100;
}

void Slider::setValue(const float& newValue) {
	sliderValue = newValue;
	localSize.x = sliderValue;
	calculateSize();
}

}
