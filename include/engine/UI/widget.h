#pragma once

#include "SDL2/SDL_render.h"
#include "engine/UI/anchorTypes.h"
#include "engine/vector2D.h"
#include <functional>
#include <list>
#include <memory>
#include <vector>

namespace UI {
// A widget without a parent should be constructed normally as an object,
// but child widgets should be made as raw pointers using the "new" keyword with
// a pointer to the parent widget as a constructor argument
class Widget {
public:
<<<<<<< HEAD
	Widget(Widget* parent = nullptr, AnchorType anchorPosition = AnchorType());
	virtual ~Widget() = default;
=======
  Widget(Widget *parent = nullptr, AnchorType anchorPosition = AnchorType());
  virtual ~Widget() = default;
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  // Should be called at the end of inheriting objects functions
  virtual void update();
  virtual void render(SDL_Renderer *renderer) const;
  std::function<void(SDL_Renderer *)> getRenderFunction() const;

  // calculateChildren means it will update children positions as well
  virtual void calculatePosition(const bool &calculateChildren = true);
  virtual void calculateSize();

  void addChild(Widget *child);

  // If widget has parent these values are based on percentage of parent size
  vector2Df localPosition;
  vector2Df localSize;

protected:
<<<<<<< HEAD
	Widget* parent;
=======
  Widget *parent;
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  vector2D position; // Rendering position, pixels
  vector2Df size;

private:
<<<<<<< HEAD
	AnchorType anchorPosition;
	std::list<std::unique_ptr<Widget>> childWidgets;
=======
  AnchorType anchorPosition;
  std::list<std::unique_ptr<Widget>> childWidgets;
>>>>>>> parent of 3336e98 (chore: fix linting issues)
};

} // namespace UI
