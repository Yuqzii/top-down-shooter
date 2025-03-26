#pragma once

#include "game/UI/canvas.h"
#include "game/vector2D.h"
#include "SDL2/SDL_render.h"

namespace UI {

class Canvas;

// Widgets will not require a canvas, but for better control it is needed
class Widget {
public:
	Widget();
	Widget(Canvas* canvas);

	virtual void update();
	virtual void render(SDL_Renderer* renderer) const;
	
	// If canvas is defined this is based on percentage of canvas size
	// otherwise it is screen position in pixels.
	vector2Df position;

protected:
	Canvas* canvas;
	
};

}
