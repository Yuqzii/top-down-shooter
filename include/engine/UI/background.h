#pragma once

#include "engine/UI/widget.h"

namespace UI {

class Background : public Widget {
public:
<<<<<<< HEAD
	Background(const vector2Df& position, const vector2Df& size, const SDL_Color& color,
			   Widget* parent = nullptr, AnchorType anchorPosition = AnchorType());
=======
  Background(const vector2Df &position, const vector2Df &size,
             const SDL_Color &color, Widget *parent = nullptr,
             AnchorType anchorPosition = AnchorType());
>>>>>>> parent of 3336e98 (chore: fix linting issues)

  void render(SDL_Renderer *renderer) const override;

  void calculatePosition(const bool &calculateChildren = true) override;
  void calculateSize() override;

private:
<<<<<<< HEAD
	SDL_Color color;
	SDL_Rect rect;
=======
  SDL_Color color;
  SDL_Rect rect;
>>>>>>> parent of 3336e98 (chore: fix linting issues)
};

} // namespace UI
