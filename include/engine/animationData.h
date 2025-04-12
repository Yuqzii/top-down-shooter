#pragma once

#include <functional>

class Scene;

struct AnimationData {
	const int length;	// Length of animation in frames
	const float speed;	// Value to multiply with deltaTime to dictate speed
};

struct AnimationEvent {
	const int sequenceId;  // Sequence this event applies to
	// The frame/time in the animation the event fires. F.ex. 5.5f for between
	// frame 5 and 6.
	const float time;
	const std::function<void(Scene&)> event;  // Function to call
};
