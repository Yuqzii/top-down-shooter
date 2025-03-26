#pragma once

#include "game/UI/widget.h"
#include "game/vector2D.h"
#include "SDL2/SDL_render.h"
#include <vector>

namespace UI {

class Widget;

// Canvas is responsible for handling all widgets underneath it
class Canvas {
public:
	vector2D getPosition() const { return position; };

	void update();
	void render(SDL_Renderer* renderer) const;
private:
	vector2D position;

	// Vector of child widgets
	std::vector<Widget> widgets;
};

}
