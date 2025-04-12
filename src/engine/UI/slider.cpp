#include "engine/UI/slider.h"

namespace UI {

Slider::Slider(const SDL_Color &color, Widget *parent)
    : Background(vector2Df(), vector2Df(100, 100), color, parent) {
  sliderValue = 100;
}

void Slider::setValue(const float &newValue) {
  sliderValue =
      std::clamp(newValue, 0.0f, 100.0f); // Clamp sliderValue between 0 and 100
  localSize.x = sliderValue;
  calculateSize();
}

} // namespace UI
