#pragma once

#include "engine/UI/background.h"

namespace UI {

class Slider : public Background {
public:
	Slider(const SDL_Color& color, Widget* parent);

	void setValue(const float& newValue);

private:
	float sliderValue;
};

}  // namespace UI
