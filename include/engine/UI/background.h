#pragma once

#include "engine/UI/widget.h"

namespace UI {

class Background : public Widget {
public:
	Background(const Vec2& position, const Vec2& size, const SDL_Color& color,
			   Widget* parent = nullptr, AnchorType anchorPosition = AnchorType());

	void render(SDL_Renderer* renderer) const override;

	void calculatePosition(const bool& calculateChildren = true) override;
	void calculateSize() override;

private:
	SDL_Color color;
	SDL_Rect rect;
};

}  // namespace UI
