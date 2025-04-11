#pragma once

#include <functional>

class Scene;

struct AnimationData {
	const int length; // Length of animation in frames
	const float speed; // Value to multiply with deltaTime to dictate speed
};

struct AnimationEvent {
	const int sequenceId;
	const int frame;
	const std::function<void(Scene&)> event;
};
