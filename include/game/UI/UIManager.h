#pragma once

#include <functional>
#include <vector>
#include "SDL2/SDL_render.h"

namespace UI {

class Widget;

class UIManager {
public:
	UIManager();

	void addRenderCall(std::function<void(SDL_Renderer*)> func);

	void update();
	void render(SDL_Renderer* renderer) const;

private:
	std::vector<std::function<void(SDL_Renderer*)>> renderCalls;
	int renderCallCnt; // Keep track of amount of calls to avoid clearing vector every frame
};

}
