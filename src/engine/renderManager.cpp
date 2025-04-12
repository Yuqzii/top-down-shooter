#include "engine/renderManager.h"
#include "engine/gameObject.h"

RenderManager::RenderManager() { renderCallCnt = 0; }

void RenderManager::addRenderCall(std::function<void(SDL_Renderer *)> func,
                                  const GameObject *parent) {
  if (renderCalls.size() > renderCallCnt) {
    // Can just reassign existing variable
    renderCalls[renderCallCnt].first = func;
    renderCalls[renderCallCnt].second = parent;
  } else {
    renderCalls.push_back(
        std::make_pair(func, parent)); // Need to add more space
  }
  renderCallCnt++;
}

void RenderManager::resetCallCnt() { renderCallCnt = 0; }

void RenderManager::update() {
  // Iterate over parents of render calls and check for deletion
  for (auto it = renderCalls.cbegin(); it != renderCalls.cend();) {
    if (it->second->deleteObject) {
      it = renderCalls.erase(it); // Remove render call
      renderCallCnt--;            // Update call count
    } else
      it++;
  }
}

void RenderManager::render(SDL_Renderer *renderer) const {
  // Iterate over all render calls and call the render function
  for (int i = 0; i < renderCallCnt; i++) {
    renderCalls[i].first(renderer);
  }
}
