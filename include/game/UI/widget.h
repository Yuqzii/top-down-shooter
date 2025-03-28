#pragma once

#include <functional>
#include <list>
#include <memory>
#include <vector>
#include "game/vector2D.h"
#include "SDL2/SDL_render.h"

namespace UI {

// A widget without a parent should be constructed normally as an object,
// but child widgets should be made as raw pointers using the "new" keyword with a pointer to the
// parent widget as a constructor argument
class Widget {
public:
	Widget();
	Widget(Widget* parent);

	// Should be called at the end of inheriting objects functions
	virtual void update();
	virtual void render(SDL_Renderer* renderer) const;
	virtual std::function<void(SDL_Renderer*)> getRenderFunction() const;

	virtual void calculatePosition();
	virtual void calculateSize();

	void addChild(Widget* child);
	
	// If widget has parent these values is based on percentage of parent size
	vector2Df localPosition;
	vector2Df localSize;
	std::list<std::unique_ptr<Widget>> childWidgets;

protected:
	Widget* parent;

	vector2D position; // Rendering position, pixels
	vector2Df size;

private:
};

}
