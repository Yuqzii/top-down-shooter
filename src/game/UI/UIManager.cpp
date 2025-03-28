#include "game/UI/UIManager.h"

namespace UI {

UIManager::UIManager() {
	renderCallCnt = 0;
}

void UIManager::addRenderCall(std::function<void(SDL_Renderer*)> func) {
	if (renderCalls.size() > renderCallCnt) {
		renderCalls[renderCallCnt] = func; // Can just reassign existing variable
	}
	else {
		renderCalls.push_back(func); // Need to add more space
	}
	renderCallCnt++;
}

void UIManager::update() {
	renderCallCnt = 0; // Reset call counter
}

void UIManager::render(SDL_Renderer* renderer) const {
	// Iterate over all render calls and call the render function
	for (int i = 0; i < renderCallCnt; i++) {
		renderCalls[i](renderer);
	}
}

}
