#pragma once

#include <vector>
#include "../managers/Sprite.hpp"

//AnimationComponent - provide animation to an entity

struct AnimationComponent
{
	std::vector<int> frames; //frames vector
	float framerate; //change frame each quantity of seconds
	float currentTime; // current elapsed time
	int currentframe; //current frame to draw
	int framewidth = 0; //width of frame to draw
	int numframes = 0; //number of animation frames
};