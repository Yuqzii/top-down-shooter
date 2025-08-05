#pragma once

#include <functional>
#include <vector>

#include "SDL2/SDL_render.h"

class GameObject;

namespace UI {

class Widget;

class UIManager {
public:
	UIManager();

	// Adds a rendering call to the list that will be rendered.
	// Also takes a GameObject pointer to the parent of the Widget
	// to check for possible deletion (fixing #29).
	void addRenderCall(std::function<void(SDL_Renderer*)> func, const GameObject* parent);

	// Resets the renderCallCnt to zero
	void resetCallCnt();
	void update();
	void render(SDL_Renderer* renderer) const;

private:
	// Render function, parent
	std::vector<std::pair<std::function<void(SDL_Renderer*)>, const GameObject*>> renderCalls;
	int renderCallCnt;  // Keep track of amount of calls to avoid clearing vector every frame
};

}  // namespace UI
