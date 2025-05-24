#pragma once

#include <functional>
#include <list>
#include <memory>

#include "SDL2/SDL_render.h"
#include "engine/UI/anchorTypes.h"
#include "engine/vector2D.h"

class Scene;

namespace UI {
// A widget without a parent should be constructed normally as an object,
// but child widgets should be made as raw pointers using the "new" keyword with a pointer to the
// parent widget as a constructor argument
class Widget {
public:
	Widget(Widget* parent = nullptr, AnchorType anchorPosition = AnchorType());
	virtual ~Widget() = default;

	// Should be called at the end of inheriting objects functions
	virtual void update();
	virtual void render(SDL_Renderer* renderer) const;
	std::function<void(Scene&)> getRenderFunction() const;

	// calculateChildren means it will update children positions as well
	virtual void calculatePosition(const bool& calculateChildren = true);
	virtual void calculateSize();

	void addChild(Widget* child);

	// If widget has parent these values are based on percentage of parent size
	Vec2 localPosition;
	Vec2 localSize;

protected:
	Widget* parent;

	Vec2 position;	// Rendering position, pixels
	Vec2 size;

private:
	AnchorType anchorPosition;
	std::list<std::unique_ptr<Widget>> childWidgets;
};

}  // namespace UI
