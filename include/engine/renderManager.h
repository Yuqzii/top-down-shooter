#pragma once

#include <functional>
#include <vector>

class GameObject;
class Scene;

class RenderManager {
public:
	RenderManager();

	// Adds a rendering call to the list that will be rendered.
	// Also takes a GameObject pointer to the parent of the Widget
	// to check for possible deletion (fixing #29).
	void addRenderCall(std::function<void(Scene&)> func, const GameObject* parent);

	// Resets the renderCallCnt to zero
	void resetCallCnt();
	void update();
	void render(Scene& scene) const;

private:
	// Render function, parent
	std::vector<std::pair<std::function<void(Scene&)>, const GameObject*>> renderCalls;
	int renderCallCnt;	// Keep track of amount of calls to avoid clearing vector every frame
};
