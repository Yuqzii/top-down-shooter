#pragma once

#include "game/UI/widget.h"

namespace UI {

class Background : public Widget {
public:
	Background(const vector2Df& position, const vector2Df& size, const SDL_Color& color,
			AnchorType anchorPosition = AnchorType(), Widget* parent = nullptr);

	void update() override;
	void render(SDL_Renderer* renderer) const override;
	std::function<void(SDL_Renderer*)> getRenderFunction() const override;

	void calculatePosition() override;
	void calculateSize() override;

private:
	SDL_Rect rect;
	SDL_Color color;
};

}
