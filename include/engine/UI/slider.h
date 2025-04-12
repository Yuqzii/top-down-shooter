#pragma once

#include "engine/UI/background.h"

namespace UI {

class Slider : public Background {
public:
<<<<<<< HEAD
	Slider(const SDL_Color& color, Widget* parent);
=======
  Slider(const SDL_Color &color, Widget *parent);
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void setValue(const float &newValue);

private:
<<<<<<< HEAD
	float sliderValue;
=======
  float sliderValue;
>>>>>>> parent of 3336e98 (chore: fix linting issues)
};

} // namespace UI
