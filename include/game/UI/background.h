#pragma once

#include "game/UI/widget.h"

namespace UI {

class Background : public Widget {
public:
	Background(vector2Df position, vector2Df size);

	void update() override;
	void render(SDL_Renderer* renderer) const override;

private:
	SDL_Rect rect;
};

}
